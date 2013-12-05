//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#include "scy/pacman/installtask.h"
#include "scy/pacman/packagemanager.h"
#include "scy/pacman/package.h"
#include "scy/archo/zip.h"
#include "scy/http/authenticator.h"
#include "scy/http/client.h"
#include "scy/logger.h"
#include "scy/filesystem.h"


using namespace std;


namespace scy { 
namespace pman {


InstallTask::InstallTask(PackageManager& manager, LocalPackage* local, RemotePackage* remote, const InstallOptions& options) :
	_manager(manager),
	_local(local),
	_remote(remote),
	_options(options),
	_dlconn(nullptr),
	_downloading(false),
	_progress(0)
{
	assert(valid());
}


InstallTask::~InstallTask()
{
	// :)
}


void InstallTask::start()
{
	log("trace") << "start" << endl;	
	Mutex::ScopedLock lock(_mutex);
	_thread.start(*this);
}


void InstallTask::cancel()
{
	setState(this, InstallationState::Cancelled, "Cancelled by user.");
}


void InstallTask::run()
{
	try {	
		// Prepare environment and install options
		{
			Mutex::ScopedLock lock(_mutex);

			// Check against provided options to make sure that
			// we can proceed with task creation.
			if (!_options.version.empty())    { _remote->assetVersion(_options.version); } // throw if none
			if (!_options.sdkVersion.empty()) { _remote->latestSDKAsset(_options.sdkVersion); }// throw if none

			// Set default install directory if none was given
			if (_options.installDir.empty()) {

				// Use the current install dir if the local package already exists
				if (!_local->installDir().empty()) {
					_options.installDir = _local->installDir();
				}

				// Or use the manager default
				else {
					_options.installDir = _manager.options().installDir;
				}
			}

			// Normalize lazy windows paths
			_options.installDir = fs::normalize(_options.installDir);
			_local->setInstallDir(_options.installDir);

			// Create the directory
			fs::mkdirr(_options.installDir);

			// If the package failed previously we might need 
			// to clear the file cache.
			if (_manager.options().clearFailedCache)
				_manager.clearPackageCache(*_local);
		}		

		// Kick off the state machine. If any errors are encountered
		// an exception will be thrown and the task will fail.
		doDownload();
		do {
			scy::sleep(50);
			if (cancelled()) goto Complete;
		} while(_downloading);

		doExtract();
		if (cancelled()) goto Complete;
		doFinalize();
			
		// Transition the internal state if finalization was a success.
		// This will complete the installation process.
		setState(this, InstallationState::Installed);
	}
	catch (std::exception& exc) {		
		ErrorL << "Installation failed: " << exc.what() << endl; 
		setState(this, InstallationState::Failed, exc.what());
	}
	
Complete:
	setComplete();
	deleteLater<InstallTask>(this);
}


void InstallTask::onStateChange(InstallationState& state, const InstallationState& oldState)
{
	DebugL << "state changed to " << state << endl; 	
	{
		auto local = this->local();
		switch (state.id()) 
		{			
		case InstallationState::Downloading:
			setProgress(0);
			break;
		case InstallationState::Extracting:
			setProgress(75);
			break;
		case InstallationState::Finalizing:
			setProgress(90);
			break;
		case InstallationState::Installed:
			local->setState("Installed");
			local->clearErrors();
			local->setInstalledAsset(getRemoteAsset());
			setProgress(100);
			break;
		case InstallationState::Cancelled:
			local->setState("Failed");
			{
				Mutex::ScopedLock lock(_mutex);
				if (_dlconn)
					_dlconn->close();
			}
			setProgress(100);
			break;			
		case InstallationState::Failed:
			local->setState("Failed");
			if (!state.message().empty())
				local->addError(state.message());
			setProgress(100);
			break;
		default: 
			local->setState("Installing");	
		}

		// Set the package install task so we know from which state to
		// resume installation.
		// TODO: Should this be reset by the clearFailedCache option?
		local->setInstallState(state.toString());
	}

	Stateful<InstallationState>::onStateChange(state, oldState);
}


void InstallTask::doDownload()
{
	setState(this, InstallationState::Downloading);

	Package::Asset asset = getRemoteAsset();
	if (!asset.valid())
		throw std::runtime_error("Package download failed: The remote asset is invalid.");

	// If the remote asset already exists in the cache, we can 
	// skip the download. 
	/* // force file re-download until os get file size is fixed and we can match crc
	if (_manager.hasCachedFile(asset)) {
		DebugL << "file exists, skipping download" << endl;		
		setState(this, InstallationState::Extracting);
		return;
	}
	*/

	std::string outfile = _manager.getCacheFilePath(asset.fileName());
	_dlconn = http::createConnection(asset.url());
	if (!_manager.options().httpUsername.empty()) {
		http::BasicAuthenticator cred(
			_manager.options().httpUsername, 
			_manager.options().httpPassword);
		cred.authenticate(_dlconn->request()); 
	}
	
	DebugL << "Initializing download:" 
		<< "\n\tURI: " << asset.url()
		<< "\n\tFile path: " << outfile
		<< endl;

	_dlconn->setReadStream(new std::ofstream(outfile, std::ios_base::out | std::ios_base::binary));
	_dlconn->IncomingProgress += delegate(this, &InstallTask::onDownloadProgress);
	_dlconn->Complete += delegate(this, &InstallTask::onDownloadComplete);
	_dlconn->send();

	_downloading = true;
}


void InstallTask::onDownloadProgress(void*, const double& progress)
{
	DebugL << "download progress: " << progress << endl;

	// Progress 1 - 75 covers download
	// Increments of 10 or greater
	int prog = static_cast<int>(progress * 0.75);
	if (prog > 0 && prog > this->progress() + 10)
		setProgress(prog);
}


void InstallTask::onDownloadComplete(void*, const http::Response& response)
{
	DebugL << "download complete: " << response << endl;
	_dlconn->readStream<std::ofstream>()->close();
	_dlconn = nullptr;
	_downloading = false;
}


void InstallTask::doExtract()
{
	setState(this, InstallationState::Extracting);
		
	Package::Asset asset = getRemoteAsset();
	if (!asset.valid())
		throw std::runtime_error("The package can't be extracted");
	
	// Get the input file and check veracity
	std::string archivePath(_manager.getCacheFilePath(asset.fileName()));
	if (!fs::exists(archivePath))
		throw std::runtime_error("The local package file does not exist: " + archivePath);	
	if (!_manager.isSupportedFileType(asset.fileName()))
		throw std::runtime_error("The local package has an unsupported file extension: " + fs::extname(archivePath));
	
	// Create the output directory
	std::string tempDir(_manager.getIntermediatePackageDir(_local->id()));
	
	DebugL << "Unpacking archive: " << archivePath << " to " << tempDir << endl;

	// Reset the local installation manifest before extraction
	_local->manifest().root.clear();
	
	// Decompress the archive
	arc::ZipFile zip(archivePath);			
	while (zip.goToNextFile()) {
		zip.extractCurrentFile(tempDir, true);
	
		// Add the extracted file to the package install manifest	
		// Note: Manifest stores relative paths
		_local->manifest().addFile(zip.currentFileName()); 
	}
}


void InstallTask::doFinalize() 
{
	setState(this, InstallationState::Finalizing);

	bool errors = false;
	std::string tempDir(_manager.getIntermediatePackageDir(_local->id()));
	std::string installDir = options().installDir;

	// Ensure the install directory exists
	fs::mkdirr(installDir);
	DebugL << "Finalizing to: " << installDir << endl;
	
	// Move all extracted files to the installation path
	StringVec nodes;
	fs::readdir(tempDir, nodes);
	for (unsigned i = 0; i < nodes.size(); i++) {
		try
		{
			std::string source(tempDir);
			fs::addnode(source, nodes[i]);

			std::string target(installDir);
			fs::addnode(target, nodes[i]);

			DebugL << "moving file: " << source << " => " << target << endl;
			fs::rename(source, target);
		}
		catch (std::exception& exc)
		{
			// The previous version files may be currently in use,
			// in which case PackageManager::finalizeInstallations()
			// must be called from an external process before the
			// installation can be completed.
			errors = true;
			ErrorL << "finalize error: " << exc.what() << endl;
			_local->addError(exc.what());
		}
	}

	// The package requires finalizing at a later date. 
	// The current task will be cancelled, and the package
	// saved with the Installing state.
	if (errors) {
		DebugL << "Finalization failed, cancelling task" << endl;
		cancel();
		return;
	}
	
	// Remove the temporary output folder if the installation
	// was successfully finalized.
	try
	{		
		DebugL << "Removing temp directory: " << tempDir << endl;

		// FIXME: How to remove a folder properly?
		fs::unlink(tempDir);
		assert(fs::exists(tempDir));
	}
	catch (std::exception& exc)
	{
		// While testing on a windows system this fails regularly
		// with a file sharing error, but since the package is already
		// installed we can just swallow it.
		WarnL << "cannot remove temp directory: " << exc.what() << endl;
	}

	DebugL << "finalization complete" << endl;
}


void InstallTask::setComplete()
{
	{
		Mutex::ScopedLock lock(_mutex);
		assert(_progress == 100);

		InfoL << "Package installed:" 
			<< "\n\tName: " << _local->name()
			<< "\n\tVersion: " << _local->version()
			<< "\n\tPackage State: " << _local->state()
			<< "\n\tPackage Install State: " << _local->installState()
			<< endl;
#ifdef _DEBUG
		_local->print(cout);	
#endif
	}
	
	// The task will be destroyed
	// as a result of this signal.
	Complete.emit(this);
}


void InstallTask::setProgress(int value) 
{
	{
		Mutex::ScopedLock lock(_mutex);	
		_progress = value;
	}
	Progress.emit(this, value);
}


Package::Asset InstallTask::getRemoteAsset() const
{
	Mutex::ScopedLock lock(_mutex);
	return !_options.version.empty() ? 
		_remote->assetVersion(_options.version) : 
			!_options.sdkVersion.empty() ?
				_remote->latestSDKAsset(_options.sdkVersion) :
					_remote->latestAsset();
}


int InstallTask::progress() const
{
	Mutex::ScopedLock lock(_mutex);
	return _progress;
}


bool InstallTask::valid() const
{
	Mutex::ScopedLock lock(_mutex);
	return !stateEquals(InstallationState::Failed) 
		&& _local->valid() 
		&& (!_remote || _remote->valid());
}


bool InstallTask::cancelled() const
{
	return stateEquals(InstallationState::Cancelled);
}


bool InstallTask::failed() const
{
	return stateEquals(InstallationState::Failed);
}


bool InstallTask::success() const
{
	return stateEquals(InstallationState::Installed);
}


bool InstallTask::complete() const
{
	return stateEquals(InstallationState::Installed) 
		|| stateEquals(InstallationState::Cancelled) 
		|| stateEquals(InstallationState::Failed);
}


LocalPackage* InstallTask::local() const
{
	Mutex::ScopedLock lock(_mutex);
	return _local;
}


RemotePackage* InstallTask::remote() const
{
	Mutex::ScopedLock lock(_mutex);
	return _remote;
}


InstallOptions& InstallTask::options() 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _options;
}


} } // namespace scy::pman



	/*
	if (!_dlconn->send() && 
		!_dlconn->cancelled())
		throw std::runtime_error(format("Cannot download package files: HTTP Error: %d %s", 
			static_cast<int>(_dlconn->response().getStatus()), 
			_dlconn->response().getReason()));
	
	DebugL << "download complete" << endl; 
			*/

		
	// Print contents to the log file
	//for (size_t i = 0; i < zip.info.size(); i++)
	//	DebugL << "Zip file contains: " << zip.info[i].path << endl;
	//zip.extractTo(tempDir);

	/*
	// Extract the archive filed. An exception will be thrown
	// if any errors are encountered and the task will fail.
	ifstream in(archivePath.toString().c_str(), ios::binary);
	Poco::Zip::Decompress c(in, tempDir);
	c.EError += Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const string> >(this, &InstallTask::onDecompressionError);
	c.EOk +=Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &InstallTask::onDecompressionOk);

	c.decompressAllFiles();
	c.EError -= Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const string> >(this, &InstallTask::onDecompressionError);
	c.EOk -=Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &InstallTask::onDecompressionOk);
	*/

/*
void InstallTask::onDecompressionError(const void*, pair<const Poco::Zip::ZipLocalFileHeader, const string>& info)
{
	ErrorL << "Decompression Error: " << info.second << endl;

	// Extraction failed, throw an exception
	throw std::runtime_error("Archive Error: Extraction failed: " + info.second);
}


void InstallTask::onDecompressionOk(const void*, pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info)
{
	DebugL << "Decompressing: " 
		<< info.second.toString() << endl; 
}
*/