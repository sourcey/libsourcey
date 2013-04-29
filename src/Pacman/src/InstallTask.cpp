//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is is distributed under a dual license that allows free, 
// open source use and closed source use under a standard commercial
// license.
//
// Non-Commercial Use:
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
// 
// Commercial Use:
// Please contact mail@sourcey.com
//


#include "Sourcey/Pacman/InstallTask.h"
#include "Sourcey/Pacman/PackageManager.h"
#include "Sourcey/Pacman/Package.h"
#include "Sourcey/Logger.h"

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/Format.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Delegate.h"
#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipArchive.h"
#include "Poco/Net/HTTPBasicCredentials.h"


using namespace std;
using namespace Poco;


namespace Sourcey { 
namespace Pacman {


InstallTask::InstallTask(PackageManager& manager, LocalPackage* local, RemotePackage* remote, const Options& options) :
	_manager(manager),
	_local(local),
	_remote(remote),
	_options(options),
	_progress(0),
	_cancelled(false)
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
	FastMutex::ScopedLock lock(_mutex);
	_thread.start(*this);
}


void InstallTask::cancel()
{
	{
		FastMutex::ScopedLock lock(_mutex);
		_cancelled = true;
		_transaction.cancel();
	}
	//_thread.join(); // Thread deleted on callback
}


void InstallTask::run()
{
	try {	

		// Check against provided options to make sure that
		// we can proceed with task creation.
		if (!_options.version.empty())
			_remote->lastestAssetForVersion(_options.version); // throw if none
		if (!_options.sdkVersion.empty())
			_remote->latestAssetForSDK(_options.sdkVersion); // throw if none

		// If the package failed previously we might need
		// to clear the file cache.
		if (_manager.options().clearFailedCache)
			_manager.clearPackageCache(*_local);
		
		// Kick off the state machine. If any errors are
		// encountered an exception will be thrown and the
		// task will fail.
		setProgress(0);
		doDownload();
		setProgress(75);
		doUnpack();
		setProgress(90);
		doFinalize();
	}
	catch (Exception& exc) {		
		log("error") << "Install Failed: " << exc.displayText() << endl; 
		setState(this, PackageInstallState::Failed, exc.displayText());
	}
	
	setProgress(100);
	setComplete();
	delete this;
}


void InstallTask::onStateChange(PackageInstallState& state, const PackageInstallState& oldState)
{
	log("debug") << "State Changed to " << state.toString() << endl; 	
	{
		FastMutex::ScopedLock lock(_mutex);

		// Check for cancellation each time the state is transitioned
		// and throw an exception to break out of the current scope.
		if (_cancelled && state.id() != PackageInstallState::Failed)
			throw Exception("Installation cancelled by user intervention.");

		// Set the package install task so we know from which state to
		// resume installation.
		// TODO: Should this be reset by the clearFailedCache option?
		_local->setInstallState(state.toString());

		switch (state.id()) {			
		case PackageInstallState::Downloading:
			break;
		case PackageInstallState::Unpacking:
			break;
		case PackageInstallState::Finalizing:
			break;
		case PackageInstallState::Installed:
			_local->setState("Installed");
			_local->clearErrors();
			_local->setVersion(_local->latestAsset().version());
			break;
		case PackageInstallState::Failed:
			_local->setState("Failed");
			if (!state.message().empty())
				_local->addError(state.message());
			break;
		default: 
			_local->setState("Installing");	
		}
	}

	StatefulSignal<PackageInstallState>::onStateChange(state, oldState);
}


void InstallTask::doDownload()
{
	setState(this, PackageInstallState::Downloading);

	//if (_local->isLatestVersion(_remote->latestAsset()))
	//	throw Exception("This local package is already up to date");
	
	//Package::Asset localAsset = _local->latestAsset();
	Package::Asset remoteAsset = !_options.version.empty() ? 
		_remote->lastestAssetForVersion(_options.version) : 
			!_options.sdkVersion.empty() ?
				_remote->latestAssetForSDK(_options.sdkVersion) :
					_remote->latestAsset();

	if (!remoteAsset.valid())
		throw Exception("Package download failed: The remote asset is invalid.");

	string uri = remoteAsset.url();
	string filename = remoteAsset.fileName();

	// If the local package manifest already has a listing of
	// the latest remote asset, and the file already exists in
	// the cache we can skip the download.
	if (!_local->assets().empty() &&
		_manager.hasCachedFile(_local->latestAsset().fileName()) && 
		_local->latestAsset() == remoteAsset) {
		log("debug") << "File exists, skipping download" << endl;		
		setState(this, PackageInstallState::Unpacking);
		return;
	}
	
	// Copy the new remote asset to our local manifest.
	// TODO: Remove current listing if any - we end up
	// with multiple entries.
	Package::Asset localAsset = _local->copyAsset(remoteAsset);
	
	log("debug") << "Initializing Download:" 
		<< "\n\tURI: " << uri
		<< "\n\tRemote Filename: " << filename
		<< "\n\tLocal Filename: " << localAsset.fileName()
		<< endl;

	// Initialize a HTTP transaction to download the file.
	// If the transaction fails an exception will be thrown.
	HTTP::Request* request = new HTTP::Request("GET", uri);	
	if (!_manager.options().httpUsername.empty()) {
		Poco::Net::HTTPBasicCredentials cred(
			_manager.options().httpUsername, 
			_manager.options().httpPassword);
		cred.authenticate(*request); 
	}

	_transaction.setRequest(request);
	_transaction.setOutputPath(_manager.getCacheFilePath(localAsset.fileName()).toString());
	_transaction.ResponseProgress += delegate(this, &InstallTask::onResponseProgress);
	if (!_transaction.send() && 
		!_transaction.cancelled())
		throw Exception(format("Failed to download package files: HTTP Error: %d %s", 
			static_cast<int>(_transaction.response().getStatus()), 
			_transaction.response().getReason()));
	
	log("debug") << "Download Complete" << endl; 
	
	// Transition the internal state since the HTTP
	// transaction was a success.
	setState(this, PackageInstallState::Unpacking);
}


void InstallTask::onResponseProgress(void* sender, HTTP::TransferState& state)
{
	log("debug") << "Download Progress: " << state.progress() << endl;

	// Progress 1 - 75 covers download
	// Increments of 10 or greater
	int prog = state.progress() * 0.75;
	if (prog > 0 && prog > progress() + 10)
		setProgress(prog);
}


void InstallTask::doUnpack()
{
	setState(this, PackageInstallState::Unpacking);
		
	Package::Asset localAsset = _local->latestAsset();
	if (!localAsset.valid())
		throw Exception("The local package can't be extracted");
	
	// Get the input file and check veracity
	Path filePath(_manager.getCacheFilePath(localAsset.fileName()));
	if (!File(filePath).exists())
		throw Exception("The local package file does not exist: " + filePath.toString());	
	if (!_manager.isSupportedFileType(localAsset.fileName()))
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
	
	// Transition the internal state on success
	setState(this, PackageInstallState::Finalizing);
}


void InstallTask::onDecompressionError(const void*, pair<const Poco::Zip::ZipLocalFileHeader, const string>& info)
{
	log("error") << "Decompression Error: " << info.second << endl;

	// Extraction failed, throw an exception
	throw Exception("Archive Error: Extraction failed: " + info.second);
}


void InstallTask::onDecompressionOk(const void*, pair<const Poco::Zip::ZipLocalFileHeader, const Poco::Path>& info)
{
	log("debug") << "Decompression Success: " 
		<< info.second.toString() << endl; 
	
	// Add the extracted file to out package manifest
	_local->manifest().addFile(info.second.toString()); 
}


void InstallTask::doFinalize() 
{
	setState(this, PackageInstallState::Finalizing);

	bool errors = false;
	Path outputDir(_manager.getIntermediatePackageDir(_local->id()));
	string installDir = _options.installDir.empty() ? 
							_manager.options().installDir : 
								_options.installDir;

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
			File(fIt.path()).moveTo(installDir);
		}
		catch (Exception& exc)
		{
			// The previous version files may be currently in use,
			// in which case PackageManager::finalizeInstallations()
			// must be called from an external process before the
			// installation can be completed.
			errors = true;
			log("error") << "Error: " << exc.displayText() << endl;
			_local->addError(exc.displayText());
		}
		
		++fIt;
	}

	// The package requires finalizing at a later date. 
	// The current task will be terminated.
	if (errors) {
		log("debug") << "Finalization failed" << endl;
		_cancelled = true;
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
		log("warn") << "Cannot remove temp directory: " << exc.displayText() << endl;
	}

	log("debug") << "Finalization Complete" << endl;
	
	// Transition the internal state if finalization was a success.
	// This will complete the installation process.
	setState(this, PackageInstallState::Installed);
}


void InstallTask::setComplete()
{
	{
		FastMutex::ScopedLock lock(_mutex);

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
		FastMutex::ScopedLock lock(_mutex);	
		_progress = value;
	}
	Progress.emit(this, value);
}


int InstallTask::progress() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _progress;
}


bool InstallTask::valid() const
{
	FastMutex::ScopedLock lock(_mutex);
	return !stateEquals(PackageInstallState::Failed) 
		&& _local->valid() 
		&& (!_remote || _remote->valid());
}


bool InstallTask::cancelled() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _cancelled;
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
		|| stateEquals(PackageInstallState::Failed);
}


LocalPackage* InstallTask::local() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _local;
}


RemotePackage* InstallTask::remote() const
{
	FastMutex::ScopedLock lock(_mutex);
	return _remote;
}


InstallTask::Options& InstallTask::options() 
{ 
	FastMutex::ScopedLock lock(_mutex);
	return _options;
}


} } // namespace Sourcey::Pacman