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

#include "Sourcey/Pacman/PackageManager.h"
#include "Sourcey/Pacman/Package.h"
#include "Sourcey/JSON/JSON.h"
#include "Sourcey/Util.h"
#include "Sourcey/HTTP/Client.h"
#include "Sourcey/HTTP/Authenticator.h"


using namespace std;


namespace scy { 
namespace pman {


//
// TODO: 
// 1) Check the integrity and version of package manifest
//		files before overwriting.
// 2) Ensure there is only a single manifest file per folder.
// 3) Generate update reports.
// 4) Generate a package profile JSON document for keeping
//	track of installed packages.
// 5) Ensure no updates are active for current package before updaing.
// 6) CRC checks.
//
	

PackageManager::PackageManager(const Options& options) :
	_options(options)
{
}


PackageManager::~PackageManager()
{
}

	
void PackageManager::initialize()
{
	createDirectories();
	loadLocalPackages();
}


bool PackageManager::initialized() const
{
	Mutex::ScopedLock lock(_mutex);
	return !_remotePackages.empty()
		|| !_localPackages.empty();
}


void PackageManager::uninitialize()
{	
	cancelAllTasks();
	
	Mutex::ScopedLock lock(_mutex);
	_remotePackages.clear();
	_localPackages.clear();
}


void PackageManager::cancelAllTasks()
{
	Mutex::ScopedLock lock(_mutex);
	auto it = _tasks.begin();
	while (it != _tasks.end()) {
		(*it)->cancel();
		it = _tasks.erase(it);
	}
}


void PackageManager::createDirectories()
{
	Mutex::ScopedLock lock(_mutex);	
	fs::mkdirr(_options.cacheDir);
	fs::mkdirr(_options.interDir);
	fs::mkdirr(_options.installDir);
}


void PackageManager::queryRemotePackages()
{	
	Mutex::ScopedLock lock(_mutex);	
	debugL("PackageManager", this) << "querying server: " 
		<< _options.endpoint << _options.indexURI << endl;

	if (!_tasks.empty())
		throw std::runtime_error("Cannot load packages while tasks are active.");	

	try {
		auto conn = new http::ClientConnection(_options.endpoint + _options.indexURI);
		conn->Complete += delegate(this, &PackageManager::onPackageQueryResponse);		
		conn->request().setMethod("GET");
		conn->request().setKeepAlive(false);
		conn->setReadStream(new std::stringstream);

		if (!_options.httpUsername.empty()) {
			http::BasicAuthenticator cred(_options.httpUsername, _options.httpPassword);
			cred.authenticate(conn->request()); 
		}

		conn->send();
	}
	catch (std::exception& exc) {
		errorL("PackageManager", this) << "Package Query Error: " << exc.what() << endl;
		throw exc;
	}
}


void PackageManager::onPackageQueryResponse(void* sender, const http::Response& response)
{
	auto conn = reinterpret_cast<http::ClientConnection*>(sender);

#ifdef _DEBUG
	traceL("PackageManager", this) << "server response: " 
		<< response << conn->readStream<std::stringstream>()->str() << endl;
#endif
			
	json::Value root;
	json::Reader reader;
	bool res = reader.parse(conn->readStream<std::stringstream>()->str(), root);
	if (!res) {
		
		// TODO: Set error state
		errorL("PackageManager", this) << "Invalid server JSON response: " << reader.getFormatedErrorMessages() << endl;
		return;
	}

	_remotePackages.clear();
		
	for (auto it = root.begin(); it != root.end(); it++) {		
		auto package = new RemotePackage(*it);
		if (!package->valid()) {
			errorL("PackageManager", this) << "Invalid package: " << package->id() << endl;
			delete package;
			continue;
		}
		_remotePackages.add(package->id(), package);
	}
	
	// Fire the event after creating packages
	RemotePackageResponse.emit(this, response);
}


void PackageManager::loadLocalPackages()
{
	std::string dir;
	{
		Mutex::ScopedLock lock(_mutex);

		if (!_tasks.empty())
			throw std::runtime_error("Cannot load packages while there are active tasks.");	

		_localPackages.clear();
		dir = _options.interDir;
	}
	loadLocalPackages(dir);
}


void PackageManager::loadLocalPackages(const std::string& dir)
{
	debugL("PackageManager", this) << "Loading manifests: " << dir << endl;		
	
	StringVec nodes;
	fs::readdir(dir, nodes);
	for (unsigned i = 0; i < nodes.size(); i++) {	
		if (nodes[i].find(".json") != std::string::npos) {		
			LocalPackage* package = nullptr;
			try {
				std::string path(dir);
				fs::addnode(path, nodes[i]);
				json::Value root;
				json::loadFile(path, root);

				debugL("PackageManager", this) << "Loading package manifest: " << path << endl;
				package = new LocalPackage(root);
				if (!package->valid()) {
					throw std::runtime_error("The local package is invalid.");
				}
				
				debugL("PackageManager", this) << "local package added: " << package->name() << endl;
				localPackages().add(package->id(), package);
			}
			catch (std::exception& exc) {
				errorL("PackageManager", this) << "cannot load local package manifest: " << exc.what() << endl;
				if (package)
					delete package;
			}
		}
	}
}


bool PackageManager::saveLocalPackages(bool whiny)
{
	traceL("PackageManager", this) << "saving local packages" << endl;

	bool res = true;
	LocalPackageMap toSave = localPackages().map();
	for (auto it = toSave.begin(); it != toSave.end(); ++it) {
		if (!saveLocalPackage(static_cast<LocalPackage&>(*it->second), whiny))
			res = false;
	}

	traceL("PackageManager", this) << "saving local packages: OK" << endl;

	return res;
}


bool PackageManager::saveLocalPackage(LocalPackage& package, bool whiny)
{
	bool res = false;
	try {
		std::string path(util::format("%s/%s.json", options().interDir.c_str(), package.id().c_str()));
		debugL("PackageManager", this) << "saving local package: " << package.id() << endl;
		json::saveFile(path, package);
		res = true;
	}
	catch (std::exception& exc) {
		errorL("PackageManager", this) << "save error: " << exc.what() << endl;
		if (whiny)
			throw exc;
	}
	return res;
}


//
//	Package installation methods
//

InstallTask* PackageManager::installPackage(const std::string& name, const InstallOptions& options) //, bool whiny
{	
	debugL("PackageManager", this) << "Install package: " << name << endl;	

	// Try to update our remote package list if none exist.
	// TODO: Consider storing a remote package cache file.
	//if (remotePackages().empty())
		//queryRemotePackages();

	// Get the asset to install or throw
	PackagePair pair = getOrCreatePackagePair(name);	
	
	// Get the asset to install or return a nullptr
	InstallOptions opts(options);
	try  {
		Package::Asset asset = getLatestInstallableAsset(pair, options);

		// Set the version option as the version to install.
		// KLUDGE: Instead of modifying options, we should have 
		// a better way of sending the asset/version to the InstallTask.
		opts.version = asset.version();
	}
	catch (std::exception& exc) {
		debugL("PackageManager", this) << "No installable assets: " << exc.what() << endl;
		return nullptr;
	}
	
	return createInstallTask(pair, opts);	
}


Package::Asset PackageManager::getLatestInstallableAsset(const PackagePair& pair, const InstallOptions& options) const
{
	if (!pair.local || !pair.remote)
		throw std::runtime_error("Must have a local and remote package to determine installable assets.");

	bool isInstalledAndVerified =
		pair.local->isInstalled() && 
		pair.local->verifyInstallManifest();
	
	debugL("PackageManager", this) << "Get asset to install:"
		<< "\n\tName: " << pair.local->name() 
		<< "\n\tVersion: " << pair.local->version() 
		<< "\n\tVersion Lock: " << pair.local->versionLock() 
		<< "\n\tSDK Version Lock: " << pair.local->sdkLockedVersion() 
		<< "\n\tVerified: " << isInstalledAndVerified
		<< endl;

	// Return true if the locked version is already installed
	if (!pair.local->versionLock().empty() || !options.version.empty()) {
		std::string versionLock = options.version.empty() ? pair.local->versionLock() : options.version;
		// TODO: assert valid version?

		// Make sure the version option matches the lock, if both were set
		if (!options.version.empty() && !pair.local->versionLock().empty() && 
			options.version != pair.local->versionLock())
			throw std::runtime_error("Invalid version option: Package locked at version: " + pair.local->versionLock());

		// If everything is in order there is nothing to install
		if (isInstalledAndVerified && pair.local->versionLock() == pair.local->version())
			throw std::runtime_error("Package is up to date. Locked at " + pair.local->versionLock());

		// Return the locked asset, or throw
		return pair.remote->assetVersion(versionLock);
	}
	
	// Get the best asset from the locked SDK version, if applicable
	if (!pair.local->sdkLockedVersion().empty() || !options.sdkVersion.empty()) {		
				
		// Make sure the version option matches the lock, if both were set
		if (!options.sdkVersion.empty() && !pair.local->sdkLockedVersion().empty() && 
			options.sdkVersion != pair.local->sdkLockedVersion())
			throw std::runtime_error("Invalid SDK version option: Package locked at SDK version: " + pair.local->sdkLockedVersion());
		
		// Get the latest asset for SDK version
		Package::Asset sdkAsset = pair.remote->latestSDKAsset(pair.local->sdkLockedVersion()); // throw if none
		
		// If everything is in order there is nothing to install
		if (isInstalledAndVerified && pair.local->asset().sdkVersion() == pair.local->sdkLockedVersion() &&
			!util::compareVersion(sdkAsset.version(), pair.local->version()))
			throw std::runtime_error("Package is up to date for SDK: " + pair.local->sdkLockedVersion());

		return sdkAsset;
	}
	
	// If all else fails return the latest asset!
	Package::Asset latestAsset = pair.remote->latestAsset();
	if (isInstalledAndVerified && !util::compareVersion(latestAsset.version(), pair.local->version()))
		throw std::runtime_error("Package is up to date.");

	return latestAsset;
}


bool PackageManager::hasAvailableUpdates(const PackagePair& pair) const
{
	try  {
		getLatestInstallableAsset(pair); // has updates or throw
		return true; // has updates
	}
	catch (std::exception&) {}
	return false; // up to date
}


bool PackageManager::installPackages(const StringVec& ids, const InstallOptions& options, InstallMonitor* monitor, bool whiny)
{	
	bool res = false;	
	try 
	{
		for (auto it = ids.begin(); it != ids.end(); ++it) {
			auto task = installPackage(*it, options); //, whiny
			if (task) {
				if (monitor)
					monitor->addTask(task); // manual start
				else task->start(); // auto start
				res = true;
			}
		}
	}
	catch (std::exception& exc) 
	{
		errorL("PackageManager", this) << "Installation failed: " << exc.what() << endl;	
		if (whiny)
			throw exc;
	}
	return res;
}


InstallTask* PackageManager::updatePackage(const std::string& name, const InstallOptions& options)
{	
	// An update action is essentially the same as an install action,
	// except we make sure local package exists before continuing.
	{
		if (!localPackages().exists(name)) {
			std::string error("Update Failed: " + name + " is not installed.");
			errorL("PackageManager", this) << "" << error << endl;	
			throw std::runtime_error(error);
			//if (whiny)
			//else
			//	return nullptr;
		}
	}

	return installPackage(name, options); //, whiny
}


bool PackageManager::updatePackages(const StringVec& ids, const InstallOptions& options, InstallMonitor* monitor, bool whiny)
{	
	// An update action is essentially the same as an install action,
	// except we make sure local package exists before continuing.
	StringVec toUpdate(ids);
	{
		for (auto it = ids.begin(); it != ids.end(); ++it) {
			if (!localPackages().exists(*it)) {
				std::string error("Cannot update " + *it + " because it's not installed.");
				errorL("PackageManager", this) << error << endl;	
				if (whiny)
					throw std::runtime_error(error);
				//else
				//	return nullptr;
			}
			else toUpdate.push_back(*it);
		}
	}
	
	return installPackages(toUpdate, options, monitor, whiny);
}


bool PackageManager::updateAllPackages(bool whiny)
{
	StringVec toUpdate;
	auto& packages = localPackages().map();
	for (auto it = packages.begin(); it != packages.end(); ++it) {	
		toUpdate.push_back(it->first);
	}
	
	InstallOptions options;
	return installPackages(toUpdate, options, nullptr, whiny);
}


bool PackageManager::uninstallPackage(const std::string& id, bool whiny)
{
	debugL("PackageManager", this) << "uninstall: " << id << endl;	
	
	try 
	{
		LocalPackage* package = localPackages().get(id, true);
		try {	
			// Delete package files from manifest
			// NOTE: If some files fail to delete we still consider the uninstall a success.	
			LocalPackage::Manifest manifest = package->manifest();
			if (!manifest.empty()) {
				for (auto it = manifest.root.begin(); it != manifest.root.end(); it++) {
					debugL("PackageManager", this) << "delete file: " << (*it).asString() << endl;	
					try {							
						//Poco::File file(package->getInstalledFilePath((*it).asString()));
						//file.remove();						
						fs::unlink(package->getInstalledFilePath((*it).asString()));
					}
					catch (std::exception& exc) {
						errorL("PackageManager", this) << "Error deleting file: " << exc.what() << endl;
					}
				}
				manifest.root.clear();
			}	
			else {
				debugL("PackageManager", this) << "uninstall: empty package manifest: " << id << endl;	
			}	
	
			// Delete package manifest file
			std::string path(options().interDir);
			fs::addnode(path, package->id() + ".json"); // manifest_
			
			debugL("PackageManager", this) << "delete manifest: " << path << endl;	
			fs::unlink(path);
		}
		catch (std::exception& exc) {
			errorL("PackageManager", this) << "nonfatal uninstall error: " << exc.what() << endl;
			// Swallow and continue...
		}

		// Set the package as Uninstalled
		package->setState("Uninstalled");

		// Notify the outside application
		PackageUninstalled.emit(this, *package);
		
		// Free package reference from memory
		localPackages().remove(package);
		delete package;
	}
	catch (std::exception& exc) 
	{
		errorL("PackageManager", this) << "fatal uninstall error: " << exc.what() << endl;
		if (whiny)
			throw exc;
		else 
			return false;
	}

	return true;
}


bool PackageManager::uninstallPackages(const StringVec& ids, bool whiny)
{	
	debugL("PackageManager", this) << "uninstall packages: " << ids.size() << endl;	
	bool res = true;
	for (auto it = ids.begin(); it != ids.end(); ++it) {
		if (!uninstallPackage(*it, whiny))
			res = false;
	}
	return res;
}


InstallTask* PackageManager::createInstallTask(PackagePair& pair, const InstallOptions& options) //const std::string& name, InstallMonitor* monitor)
{	
	infoL("PackageManager", this) << "Create install task: " << pair.name() << endl;	

	// Ensure we only have one task per package
	if (getInstallTask(pair.remote->id()))
		throw std::runtime_error(pair.remote->name() + " is already installing.");

	auto task = new InstallTask(*this, pair.local, pair.remote, options);
	task->Complete += delegate(this, &PackageManager::onPackageInstallComplete, -1); // lowest priority to remove task
	{
		Mutex::ScopedLock lock(_mutex);
		_tasks.push_back(task);
	}
	InstallTaskCreated.emit(this, *task);
	return task; // must call task->start()
}


bool PackageManager::hasUnfinalizedPackages()
{	
	//Mutex::ScopedLock lock(_mutex);

	debugL("PackageManager", this) << "checking for unfinalized packages" << endl;
	
	bool res = false;	
	auto& packages = localPackages().map();
	for (auto it = packages.begin(); it != packages.end(); ++it) {
		if (it->second->state() == "Installing" && 
			it->second->installState() == "Finalizing") {
			debugL("PackageManager", this) << "finalization required: " << it->second->name() << endl;
			res = true;
		}
	}

	return res;
}


bool PackageManager::finalizeInstallations(bool whiny)
{
	debugL("PackageManager", this) << "Finalizing installations" << endl;
	
	bool res = true;	
	auto& packages = localPackages().map();
	for (auto it = packages.begin(); it != packages.end(); ++it) {
		try {
			if (it->second->state() == "Installing" && 
				it->second->installState() == "Finalizing") {
				debugL("PackageManager", this) << "Finalizing: " << it->second->name() << endl;

				// Create an install task on the stack - we only have
				// to move some files around so no async required.
				InstallTask task(*this, it->second, nullptr);
				task.doFinalize();
				
				assert(it->second->state() == "Installed" 
					&& it->second->installState() == "Installed");
				
				// Manually emit the install complete signal.
				InstallTaskComplete.emit(this, task);

				/*
				if (it->second->state() != "Installed" ||
					it->second->installState() != "Installed") {
					res = false;
					if (whiny)
						throw std::runtime_error(it->second->name() + ": Finalization failed");
				}
				*/
			}
		}
		catch (std::exception& exc) {
			errorL("PackageManager", this) << "Finalize Error: " << exc.what() << endl;
			res = false;
			if (whiny)
				throw exc;
		}

		// Always save the package.
		saveLocalPackage(*it->second, false);
	}

	return res;
}

void PackageManager::onPackageInstallComplete(void* sender)
{
	auto task = reinterpret_cast<InstallTask*>(sender);	
	traceL("PackageManager", this) << "Install complete: " 
		<< task->state().toString() << endl;
	
	// Save the local package
	saveLocalPackage(*task->local());

	//PackageInstallationComplete.emit(this, *task->local());
	InstallTaskComplete.emit(this, *task);

	// Remove the task reference
	{
		Mutex::ScopedLock lock(_mutex);
		for (auto it = _tasks.begin(); it != _tasks.end(); it++) {
			if (*it == task) {
				_tasks.erase(it);
				break;
			}
		}
	}

}



//
// Task helper methods
//

InstallTask* PackageManager::getInstallTask(const std::string& id) const
{
	Mutex::ScopedLock lock(_mutex);
	for (auto it = _tasks.begin(); it != _tasks.end(); it++) {
		if ((*it)->remote()->id() == id)
			return *it;
	}
	return nullptr;
}


InstallTaskList PackageManager::tasks() const
{
	Mutex::ScopedLock lock(_mutex);
	return _tasks;
}


//
// Package helper methods
//

PackagePair PackageManager::getPackagePair(const std::string& id, bool whiny) const
{		
	Mutex::ScopedLock lock(_mutex);
	auto local = _localPackages.get(id, false);
	auto remote = _remotePackages.get(id, false);

	if (whiny && local && !local->valid())
		throw std::runtime_error("The local package is invalid");

	if (whiny && remote && !remote->valid())
		throw std::runtime_error("The remote package is invalid");

	return PackagePair(local, remote);
}


PackagePairVec PackageManager::getPackagePairs() const
{	
	PackagePairVec pairs;
	Mutex::ScopedLock lock(_mutex);
	auto lpackages = _localPackages.map(); // copy
	auto rpackages = _remotePackages.map(); // copy
	for (auto lit = lpackages.begin(); lit != lpackages.end(); ++lit) {
		pairs.push_back(PackagePair(lit->second));
	}
	for (auto rit = rpackages.begin(); rit != rpackages.end(); ++rit) {
		bool exists = false;
		for (unsigned i = 0; i < pairs.size(); i++) {
			if (pairs[i].id() == rit->second->id()) {
				pairs[i].remote = rit->second;
				exists = true;
				break;
			}
		}
		if (!exists)
			pairs.push_back(PackagePair(nullptr, rit->second));
	}
	return pairs;
}


PackagePairVec PackageManager::getUpdatablePackagePairs() const
{
	PackagePairVec pairs = getPackagePairs();		
	for (auto it = pairs.begin(); it != pairs.end();) {
		if (!hasAvailableUpdates(*it)) {	
			it = pairs.erase(it);
		}
		else ++it;
	}
	return pairs;
}


PackagePair PackageManager::getOrCreatePackagePair(const std::string& id)
{	
	Mutex::ScopedLock lock(_mutex);
	auto remote = _remotePackages.get(id, false);
	if (!remote)
		throw std::runtime_error("The remote package does not exist.");

	if (remote->assets().empty())
		throw std::runtime_error("The remote package has no file assets.");

	if (!remote->latestAsset().valid())
		throw std::runtime_error("The remote package has invalid file assets.");

	if (!remote->valid())
		throw std::runtime_error("The remote package is invalid.");

	// Get or create the local package description.
	auto local = _localPackages.get(id, false);
	if (!local) {
		local = new LocalPackage(*remote);
		_localPackages.add(id, local);
	}
	
	if (!local->valid())
		throw std::runtime_error("The local package is invalid.");
	
	return PackagePair(local, remote);
}


string PackageManager::installedPackageVersion(const std::string& id) const
{
	Mutex::ScopedLock lock(_mutex);
	auto local = _localPackages.get(id, true);
	
	if (!local->valid())
		throw std::runtime_error("The local package is invalid.");
	if (!local->isInstalled())
		throw std::runtime_error("The local package is not installed.");

	return local->version();
}


#if 0
bool PackageManager::hasAvailableUpdates(PackagePair& pair)
{		
	assert(pair.valid());
	return pair.local && pair.remote && 
		pair.local->hasAvailableUpdates(*pair.remote);
}


bool PackageManager::verifyInstallManifest(LocalPackage& package)
{	
	debugL("PackageManager", this) << package.name() 
		<< ": Verifying install manifest" << endl;

	// Check file system for each manifest file
	LocalPackage::Manifest manifest = package.manifest();
	for (auto it = manifest.root.begin(); it != manifest.root.end(); it++) {		
		std::string path = package.getInstalledFilePath((*it).asString(), false);
		debugL("PackageManager", this) << package.name() 
			<< ": Checking: " << path << endl;
		File file(path);
		if (!file.exists()) {
			errorL("PackageManager", this) << package.name() 
				<< ": Missing package file: " << path << endl;
			return false;
		}
	}
	
	return true;
}
#endif
	

// ---------------------------------------------------------------------
// File Helper Methods
//
void PackageManager::clearCache()
{
	std::string dir(options().cacheDir);
	fs::addsep(dir);
	fs::rmdir(dir); // remove it
	assert(!fs::exists(dir));
}


bool PackageManager::clearPackageCache(LocalPackage& package)
{
	bool res = true;
	json::Value& assets = package["assets"];
	for (unsigned i = 0; i < assets.size(); i++) {
		Package::Asset asset(assets[i]);
		if (!clearCacheFile(asset.fileName(), false))
			res = false;
	}
	return res;
}


bool PackageManager::clearCacheFile(const std::string& fileName, bool whiny)
{
	try {
		std::string path(options().cacheDir);
		fs::addnode(path, fileName);
		fs::unlink(path);
		return true;
	}
	catch (std::exception& exc) {
		errorL("PackageManager", this) << "Clear Cache Error: " 
			<< fileName << ": " << exc.what() << endl;
		if (whiny)
			throw exc;
	}
	return false;
}


bool PackageManager::hasCachedFile(Package::Asset& asset)
{
	std::string path(options().cacheDir);
	fs::addnode(path, asset.fileName());
	return fs::exists(path); // TODO: crc and size check
}


bool PackageManager::isSupportedFileType(const std::string& fileName)
{
	return fileName.find(".zip") != std::string::npos  
		|| fileName.find(".tar.gz") != std::string::npos;
}


std::string PackageManager::getCacheFilePath(const std::string& fileName)
{
	std::string dir(options().cacheDir);
	fs::addnode(dir, fileName);
	return dir;
}

	
std::string PackageManager::getIntermediatePackageDir(const std::string& id)
{
	std::string dir(options().interDir);
	fs::addnode(dir, id);
	fs::mkdirr(dir); // create it
	return dir;
}


PackageManager::Options& PackageManager::options() 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _options;
}


RemotePackageStore& PackageManager::remotePackages() 
{ 
	Mutex::ScopedLock lock(_mutex);
	return _remotePackages; 
}


LocalPackageStore& PackageManager::localPackages()
{ 
	Mutex::ScopedLock lock(_mutex);
	return _localPackages; 
}


} } // namespace scy::pman