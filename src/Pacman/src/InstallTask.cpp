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


#include "Sourcey/Pacman/InstallTask.h"
#include "Sourcey/Pacman/PackageManager.h"
#include "Sourcey/Pacman/Package.h"
#include "Sourcey/HTTP/Authenticator.h"
#include "Sourcey/Logger.h"

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Delegate.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipArchive.h"
////#include "Poco/Net/HTTPBasicCredentials.h"


using namespace std;
using namespace Poco;


namespace scy { 
namespace pman {


InstallTask::InstallTask(PackageManager& manager, LocalPackage* local, RemotePackage* remote, const Options& options) :
	_manager(manager),
	_local(local),
	_remote(remote),
	_options(options),
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
	log("trace") << "Starting" << endl;	
	ScopedLock lock(_mutex);
	_thread.start(*this);
}


void InstallTask::cancel()
{
	setState(this, PackageInstallState::Cancelled, "Cancelled by user.");
}


bool InstallTask::run()
{
	try {	
		// Prepare environment and install options
		{
			ScopedLock lock(_mutex);

			// Check against provided options to make sure that
			// we can proceed with task creation.
			if (!_options.version.empty())
				_remote->assetVersion(_options.version); // throw if none
			if (!_options.sdkVersion.empty())
				_remote->latestSDKAsset(_options.sdkVersion); // throw if none

			// Set default install directory if none was given
			if (_options.installDir.empty()) {
				
				// Use the current install dir if the local package already exists
				if (!_local->installDir().empty()) {
					_options.installDir = _local->installDir();
				}

				// Or use the manager default
				else
					_options.installDir = _manager.options().installDir;
			}
			_local->setInstallDir(_options.installDir);
			log("debug") << "Installing To: " << _local->installDir() << endl;

			// If the package failed previously we might need
			// to clear the file cache.
			if (_manager.options().clearFailedCache)
				_manager.clearPackageCache(*_local);
		}
		
		// Kick off the state machine. If any errors are
		// encountered an exception will be thrown and the
		// task will fail.
		doDownload();
		if (cancelled()) goto Complete;
		doUnpack();
		if (cancelled()) goto Complete;
		doFinalize();
			
		// Transition the internal state if finalization was a success.
		// This will complete the installation process.
		setState(this, PackageInstallState::Installed);
	}
	catch (Exception& exc) {		
		log("error") << "Install Failed: " << exc.message() << endl; 
		setState(this, PackageInstallState::Failed, exc.message());
	}
	
Complete:
	setComplete();
	delete this;
	return false;
}


void InstallTask::onStateChange(PackageInstallState& state, const PackageInstallState& oldState)
{
	log("debug") << "State Changed to " << state.toString() << endl; 	
	{
		LocalPackage* local = this->local();
		switch (state.id()) 
		{			
		case PackageInstallState::Downloading:
			setProgress(0);
			break;
		case PackageInstallState::Unpacking:
			setProgress(75);
			break;
		case PackageInstallState::Finalizing:
			setProgress(90);
			break;
		case PackageInstallState::Installed:
			local->setState("Installed");
			local->clearErrors();
			local->setInstalledAsset(getRemoteAsset());
			setProgress(100);
			break;
		case PackageInstallState::Cancelled:
			local->setState("Failed");
			{
				ScopedLock lock(_mutex);
				_transaction->close();
			}
			setProgress(100);
			break;			
		case PackageInstallState::Failed:
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

	StatefulSignal<PackageInstallState>::onStateChange(state, oldState);
}


void InstallTask::doDownload()
{
	setState(this, PackageInstallState::Downloading);

	Package::Asset asset = getRemoteAsset();
	if (!asset.valid())
		throw Exception("Package download failed: The remote asset is invalid.");

	// If the remote asset already exists in the cache, we can 
	// skip the download. 
	if (_manager.hasCachedFile(asset)) {
		log("debug") << "File exists, skipping download" << endl;		
		setState(this, PackageInstallState::Unpacking);
		return;
	}
	
	log("debug") << "Initializing Download:" 
		<< "\n\tURI: " << asset.url()
		<< "\n\tFilename: " << asset.fileName()
		<< endl;

	// Initialize a HTTP transaction to download the file.
	// If the transaction fails an exception will be thrown.
	//http::Request* request = new http::Request("GET", asset.url());	
	
	_transaction = new http::ClientConnection(asset.url());
	if (!_manager.options().httpUsername.empty()) {
		http::BasicAuthenticator cred(
			_manager.options().httpUsername, 
			_manager.options().httpPassword);
		cred.authenticate(_transaction->request()); 
	}

	string outfile = _manager.getCacheFilePath(asset.fileName()).toString();
	_transaction->setReadStream(new std::ofstream(outfile.c_str(), std::ios_base::out | std::ios_base::binary));
	_transaction->IncomingProgress += delegate(this, &InstallTask::onIncomingProgress);
	_transaction->send();

	/*
	if (!_transaction->send() && 
		!_transaction->cancelled())
		throw Exception(format("Cannot download package files: HTTP Error: %d %s", 
			static_cast<int>(_transaction->response().getStatus()), 
			_transaction->response().getReason()));
			*/
	
	log("debug") << "Download Complete" << endl; 
}


void InstallTask::onIncomingProgress(void* sender, const double& progress)
{
	log("debug") << "Download Progress: " << progress << endl;

	// Progress 1 - 75 covers download
	// Increments of 10 or greater
	int prog = static_cast<int>(progress * 0.75);
	if (prog > 0 && prog > this->progress() + 10)
		setProgress(prog);
}


void InstallTask::doUnpack()
{
	setState(this, PackageInstallState::Unpacking);
		
	Package::Asset asset = getRemoteAsset();
	if (!asset.valid())
		throw Exception("The package can't be extracted");
	
	// Get the input file and check veracity
	Path filePath(_manager.getCacheFilePath(asset.fileName()));
	if (!File(filePath).exists())
		throw Exception("The local package file does not exist: " + filePath.toString());	
	if (!_manager.isSupportedFileType(asset.fileName()))
		throw Exception("The local package has an unsupported file extension: " + filePath.getExtension());
	
	// Create the output directory
	Path outputDir(_manager.getIntermediatePackageDir(_local->id()));
	
	log("debug") << "Unpacking: " 
		<< filePath.toString() << " to "
		<< outputDir.toString() << endl;

	// Clear the local installation manifest before extraction
	_local->manifest().root.clear();
	
	// Extract the archive filed. An exception will be thrown
	// if any errors are encountered and the task will fail.
	ifstream in(filePath.toString().c_str(), ios::binary);
	Poco::Zip::Decompress c(in, outputDir);
	c.EError += Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const string> >(this, &InstallTask::onDecompressionError);
	c.EOk +=Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &InstallTask::onDecompressionOk);

	c.decompressAllFiles();
	c.EError -= Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const string> >(this, &InstallTask::onDecompressionError);
	c.EOk -=Poco::Delegate<InstallTask, pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path> >(this, &InstallTask::onDecompressionOk);
}


void InstallTask::onDecompressionError(const void*, pair<const Poco::Zip::ZipLocalFileHeader, const string>& info)
{
	log("error") << "Decompression Error: " << info.second << endl;

	// Extraction failed, throw an exception
	throw Exception("Archive Error: Extraction failed: " + info.second);
}


void InstallTask::onDecompressionOk(const void*, pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info)
{
	log("debug") << "Decompressing: " 
		<< info.second.toString() << endl; 
	
	// Add the extracted file to out package manifest
	_local->manifest().addFile(info.second.toString()); 
}


void InstallTask::doFinalize() 
{
	setState(this, PackageInstallState::Finalizing);

	bool errors = false;
	Path outputDir(_manager.getIntermediatePackageDir(_local->id()));
	string installDir = options().installDir;

	// Ensure the install directory exists
	File(installDir).createDirectories();

	// Move all extracted files to the installation path
	DirectoryIterator fIt(outputDir);
	DirectoryIterator fEnd;
	while (fIt != fEnd)
	{
		try
		{
			log("debug") << "Moving: " << fIt.path().toString() << " <<=>> " << installDir  << endl;
			File(fIt.path()).moveTo(options().installDir);
		}
		catch (Exception& exc)
		{
			// The previous version files may be currently in use,
			// in which case PackageManager::finalizeInstallations()
			// must be called from an external process before the
			// installation can be completed.
			errors = true;
			log("error") << "Error: " << exc.message() << endl;
			_local->addError(exc.message());
		}
		
		++fIt;
	}

	// The package requires finalizing at a later date. 
	// The current task will be canceled, and the package
	// saved with the Installing state.
	if (errors) {
		log("debug") << "Finalization failed, cancelling task" << endl;
		cancel();
		return;
	}
	
	// Remove the temporary output folder if the installation
	// was successfully finalized.
	try
	{		
		log("debug") << "Removing temp directory: " << outputDir.toString() << endl;

		// FIXME: How to remove a folder properly?
		File(outputDir).remove(true);
	}
	catch (Exception& exc)
	{
		// While testing on a windows system this fails regularly
		// with a file sharing error, but since the package is already
		// installed we can just swallow it.
		log("warn") << "Cannot remove temp directory: " << exc.message() << endl;
	}

	log("debug") << "Finalization Complete" << endl;
}


void InstallTask::setComplete()
{
	{
		ScopedLock lock(_mutex);
		assert(_progress == 100);

		log("info") << "Package Install Complete:" 
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
		ScopedLock lock(_mutex);	
		_progress = value;
	}
	Progress.emit(this, value);
}


Package::Asset InstallTask::getRemoteAsset() const
{
	ScopedLock lock(_mutex);
	return !_options.version.empty() ? 
		_remote->assetVersion(_options.version) : 
			!_options.sdkVersion.empty() ?
				_remote->latestSDKAsset(_options.sdkVersion) :
					_remote->latestAsset();
}


int InstallTask::progress() const
{
	ScopedLock lock(_mutex);
	return _progress;
}


bool InstallTask::valid() const
{
	ScopedLock lock(_mutex);
	return !stateEquals(PackageInstallState::Failed) 
		&& _local->valid() 
		&& (!_remote || _remote->valid());
}


bool InstallTask::cancelled() const
{
	return stateEquals(PackageInstallState::Cancelled);
}


bool InstallTask::failed() const
{
	return stateEquals(PackageInstallState::Failed);
}


bool InstallTask::success() const
{
	return stateEquals(PackageInstallState::Installed);
}


bool InstallTask::complete() const
{
	return stateEquals(PackageInstallState::Installed) 
		|| stateEquals(PackageInstallState::Cancelled) 
		|| stateEquals(PackageInstallState::Failed);
}


LocalPackage* InstallTask::local() const
{
	ScopedLock lock(_mutex);
	return _local;
}


RemotePackage* InstallTask::remote() const
{
	ScopedLock lock(_mutex);
	return _remote;
}


InstallTask::Options& InstallTask::options() 
{ 
	ScopedLock lock(_mutex);
	return _options;
}


} } // namespace scy::pman




	//if (_local->isUpToDate(_remote->latestAsset()))
	//	throw Exception("This local package is is up to date");	
	//Package::Asset localAsset = _local->latestAsset();

	// If the local package manifest already has a listing of
	// the latest remote asset, and the file already exists in
	// the cache we can skip the download.

	//string uri = asset.url();
	//string filename = asset.fileName();

	/* //.fileName()
	if (!_local->assets().empty() &&
		_manager.hasCachedFile(_local->latestAsset().fileName()) && 
		_local->latestAsset() == asset) {
		log("debug") << "File exists, skipping download" << endl;		
		setState(this, PackageInstallState::Unpacking);
		return;
	}
	
	// Copy the new remote asset to our local manifest.
	// TODO: Remove current listing if any - we end up with multiple entries.
	// NOTE: localAsset only gets set when installed
	//Package::Asset localAsset = _local->copyAsset(asset);
	*/