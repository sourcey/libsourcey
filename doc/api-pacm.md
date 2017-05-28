# Module <!-- group --> `pacm`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::pacm`](#namespacescy_1_1pacm)    | 
`struct `[`scy::pacm::Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)    | 
`struct `[`scy::pacm::LocalPackage::Manifest`](#structscy_1_1pacm_1_1LocalPackage_1_1Manifest)    | 
`struct `[`scy::pacm::PackageManager::Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)    | [Package](#structscy_1_1pacm_1_1Package) manager initialization options.
# namespace `scy::pacm` 



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::pacm::InstallMonitor`](#classscy_1_1pacm_1_1InstallMonitor)    | 
`class `[`scy::pacm::InstallTask`](#classscy_1_1pacm_1_1InstallTask)    | This class implements the package installation procedure.
`class `[`scy::pacm::PackageManager`](#classscy_1_1pacm_1_1PackageManager)    | 
`struct `[`scy::pacm::InstallationState`](#structscy_1_1pacm_1_1InstallationState)    | 
`struct `[`scy::pacm::InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)    | [Package](#structscy_1_1pacm_1_1Package) installation options.
`struct `[`scy::pacm::LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)    | 
`struct `[`scy::pacm::Package`](#structscy_1_1pacm_1_1Package)    | 
`struct `[`scy::pacm::PackagePair`](#structscy_1_1pacm_1_1PackagePair)    | 
`struct `[`scy::pacm::RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)    | 
# class `scy::pacm::InstallMonitor` 






## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal](#classscy_1_1Signal)< void([InstallTask](#classscy_1_1pacm_1_1InstallTask) &, const [InstallationState](#structscy_1_1pacm_1_1InstallationState) &, const [InstallationState`](#structscy_1_1pacm_1_1InstallationState) &)`> InstallStateChange` | Proxies state change events from managed packages.
`public `[`Signal](#classscy_1_1Signal)< void([LocalPackage`](#structscy_1_1pacm_1_1LocalPackage) &)`> InstallComplete` | Signals when a managed install task completes.
`public `[`Signal`](#classscy_1_1Signal)< void(int &)`> Progress` | Signals on overall progress update [0-100].
`public `[`Signal`](#classscy_1_1Signal)< void(LocalPackageVec &)`> Complete` | Signals on all tasks complete.
`public  InstallMonitor()` | 
`public virtual  ~InstallMonitor()` | 
`public virtual void addTask(InstallTask::Ptr task)` | Adds a task to monitor.
`public virtual void startAll()` | Starts all monitored tasks.
`public virtual void cancelAll()` | Cancels all monitored tasks.
`public virtual bool isComplete() const` | 
`public virtual InstallTaskPtrVec tasks() const` | Returns the list of monitored package tasks.
`public virtual LocalPackageVec packages() const` | Returns the list of monitored packages.
`protected mutable std::mutex _mutex` | 
`protected InstallTaskPtrVec _tasks` | 
`protected LocalPackageVec _packages` | 
`protected int _progress` | 
`protected virtual void onInstallStateChange(void * sender,`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` | 
`protected virtual void onInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` | 
`protected virtual void setProgress(int value)` | 

## Members

#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask](#classscy_1_1pacm_1_1InstallTask) &, const [InstallationState](#structscy_1_1pacm_1_1InstallationState) &, const [InstallationState`](#structscy_1_1pacm_1_1InstallationState) &)`> InstallStateChange` 

Proxies state change events from managed packages.



#### `public `[`Signal](#classscy_1_1Signal)< void([LocalPackage`](#structscy_1_1pacm_1_1LocalPackage) &)`> InstallComplete` 

Signals when a managed install task completes.



#### `public `[`Signal`](#classscy_1_1Signal)< void(int &)`> Progress` 

Signals on overall progress update [0-100].



#### `public `[`Signal`](#classscy_1_1Signal)< void(LocalPackageVec &)`> Complete` 

Signals on all tasks complete.



#### `public  InstallMonitor()` 





#### `public virtual  ~InstallMonitor()` 





#### `public virtual void addTask(InstallTask::Ptr task)` 

Adds a task to monitor.



#### `public virtual void startAll()` 

Starts all monitored tasks.



#### `public virtual void cancelAll()` 

Cancels all monitored tasks.



#### `public virtual bool isComplete() const` 



Returns true if all install tasks have completed, either successfully or unsuccessfully.

#### `public virtual InstallTaskPtrVec tasks() const` 

Returns the list of monitored package tasks.



#### `public virtual LocalPackageVec packages() const` 

Returns the list of monitored packages.



#### `protected mutable std::mutex _mutex` 





#### `protected InstallTaskPtrVec _tasks` 





#### `protected LocalPackageVec _packages` 





#### `protected int _progress` 





#### `protected virtual void onInstallStateChange(void * sender,`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` 





#### `protected virtual void onInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` 





#### `protected virtual void setProgress(int value)` 





# class `scy::pacm::InstallTask` 

```
class scy::pacm::InstallTask
  : public scy::basic::Runnable
  : public scy::Stateful< InstallationState >
```  

This class implements the package installation procedure.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &, int &)`> Progress` | Signals on progress update [0-100].
`public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> Complete` | 
`public  InstallTask(`[`PackageManager`](#classscy_1_1pacm_1_1PackageManager)` & manager,`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local,`[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,uv::Loop * loop)` | 
`public virtual  ~InstallTask()` | 
`public virtual void start()` | 
`public virtual void cancel()` | 
`public virtual void doDownload()` | Downloads the package archive from the server.
`public virtual void doExtract()` | 
`public virtual void doFinalize()` | 
`public virtual void setComplete()` | 
`public virtual `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` getRemoteAsset() const` | 
`public virtual `[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local() const` | 
`public virtual `[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote() const` | 
`public virtual `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options()` | 
`public virtual uv::Loop * loop() const` | 
`public virtual bool valid() const` | 
`public virtual bool cancelled() const` | Returns true when the task has been cancelled.
`public virtual bool failed() const` | 
`public virtual bool success() const` | 
`public virtual bool complete() const` | 
`public virtual int progress() const` | 
`protected mutable std::mutex _mutex` | 
`protected `[`Idler`](./doc/api-base.md#classscy_1_1Idler)` _runner` | 
`protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` | 
`protected `[`PackageManager`](./doc/api-pacm.md#classscy_1_1pacm_1_1PackageManager)` & _manager` | 
`protected `[`LocalPackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1LocalPackage)` * _local` | 
`protected `[`RemotePackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1RemotePackage)` * _remote` | 
`protected `[`InstallOptions`](./doc/api-pacm.md#structscy_1_1pacm_1_1InstallOptions)` _options` | 
`protected int _progress` | 
`protected bool _downloading` | 
`protected http::ClientConnection::Ptr _dlconn` | 
`protected uv::Loop * _loop` | 
`protected virtual void run()` | Called asynchronously by the thread to do the work.
`protected virtual void onStateChange(`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` | 
`protected virtual void onDownloadProgress(const double & progress)` | 
`protected virtual void onDownloadComplete(const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | 
`protected virtual void setProgress(int value)` | 

## Members

#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &, int &)`> Progress` 

Signals on progress update [0-100].



#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> Complete` 



Signals on task completion for both success and failure cases.

#### `public  InstallTask(`[`PackageManager`](#classscy_1_1pacm_1_1PackageManager)` & manager,`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local,`[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,uv::Loop * loop)` 





#### `public virtual  ~InstallTask()` 





#### `public virtual void start()` 





#### `public virtual void cancel()` 





#### `public virtual void doDownload()` 

Downloads the package archive from the server.



#### `public virtual void doExtract()` 



Extracts the downloaded package files to the intermediate directory.

#### `public virtual void doFinalize()` 



Moves extracted files from the intermediate directory to the installation directory.

#### `public virtual void setComplete()` 



Called when the task completes either successfully or in error. This will trigger destruction.

#### `public virtual `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` getRemoteAsset() const` 





#### `public virtual `[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local() const` 





#### `public virtual `[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote() const` 





#### `public virtual `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options()` 





#### `public virtual uv::Loop * loop() const` 





#### `public virtual bool valid() const` 





#### `public virtual bool cancelled() const` 

Returns true when the task has been cancelled.



#### `public virtual bool failed() const` 





#### `public virtual bool success() const` 





#### `public virtual bool complete() const` 





#### `public virtual int progress() const` 





#### `protected mutable std::mutex _mutex` 





#### `protected `[`Idler`](./doc/api-base.md#classscy_1_1Idler)` _runner` 





#### `protected `[`scy::Error`](./doc/api-base.md#structscy_1_1Error)` _error` 





#### `protected `[`PackageManager`](./doc/api-pacm.md#classscy_1_1pacm_1_1PackageManager)` & _manager` 





#### `protected `[`LocalPackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1LocalPackage)` * _local` 





#### `protected `[`RemotePackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1RemotePackage)` * _remote` 





#### `protected `[`InstallOptions`](./doc/api-pacm.md#structscy_1_1pacm_1_1InstallOptions)` _options` 





#### `protected int _progress` 





#### `protected bool _downloading` 





#### `protected http::ClientConnection::Ptr _dlconn` 





#### `protected uv::Loop * _loop` 





#### `protected virtual void run()` 

Called asynchronously by the thread to do the work.



#### `protected virtual void onStateChange(`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` 





#### `protected virtual void onDownloadProgress(const double & progress)` 





#### `protected virtual void onDownloadComplete(const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` 





#### `protected virtual void setProgress(int value)` 





# class `scy::pacm::PackageManager` 




The [Package](#structscy_1_1pacm_1_1Package) Manager provides an interface for managing, installing, updating and uninstalling Pacm packages.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Signal](#classscy_1_1Signal)< void(const [http::Response`](#classscy_1_1http_1_1Response) &)`> RemotePackageResponse` | Events.
`public `[`Signal](#classscy_1_1Signal)< void([LocalPackage`](#structscy_1_1pacm_1_1LocalPackage) &)`> PackageUninstalled` | Signals when a package is uninstalled.
`public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> InstallTaskCreated` | 
`public `[`Signal](#classscy_1_1Signal)< void(const [InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> InstallTaskComplete` | 
`public  PackageManager(const `[`Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)` & options)` | 
`public virtual  ~PackageManager()` | 
`public virtual void initialize()` | Initialization Methods.
`public virtual void uninitialize()` | 
`public virtual bool initialized() const` | 
`public virtual void createDirectories()` | 
`public virtual void queryRemotePackages()` | Queries the server for a list of available packages.
`public virtual void loadLocalPackages()` | 
`public virtual void loadLocalPackages(const std::string & dir)` | 
`public virtual bool saveLocalPackages(bool whiny)` | 
`public virtual bool saveLocalPackage(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` & package,bool whiny)` | Saves the local package manifest to the file system.
`public virtual void parseRemotePackages(const std::string & data)` | Parse the remote packages from the given JSON data string.
`public virtual InstallTask::Ptr installPackage(const std::string & name,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` | [Package](./doc/api-pacm.md#structscy_1_1pacm_1_1Package) Installation Methods.
`public virtual bool installPackages(const StringVec & ids,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,`[`InstallMonitor`](#classscy_1_1pacm_1_1InstallMonitor)` * monitor,bool whiny)` | 
`public virtual InstallTask::Ptr updatePackage(const std::string & name,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` | 
`public virtual bool updatePackages(const StringVec & ids,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,`[`InstallMonitor`](#classscy_1_1pacm_1_1InstallMonitor)` * monitor,bool whiny)` | 
`public virtual bool updateAllPackages(bool whiny)` | Updates all installed packages.
`public virtual bool uninstallPackages(const StringVec & ids,bool whiny)` | Uninstalls multiple packages.
`public virtual bool uninstallPackage(const std::string & id,bool whiny)` | Uninstalls a single package.
`public virtual bool hasUnfinalizedPackages()` | 
`public virtual bool finalizeInstallations(bool whiny)` | 
`public virtual InstallTask::Ptr getInstallTask(const std::string & id) const` | [Task](./doc/api-base.md#classscy_1_1Task) Helper Methods.
`public virtual InstallTaskPtrVec tasks() const` | Returns a list of all tasks.
`public virtual void cancelAllTasks()` | 
`public virtual PackagePairVec getPackagePairs() const` | [Package](./doc/api-pacm.md#structscy_1_1pacm_1_1Package) Helper Methods.
`public virtual PackagePairVec getUpdatablePackagePairs() const` | 
`public virtual `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` getPackagePair(const std::string & id,bool whiny) const` | 
`public virtual `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` getOrCreatePackagePair(const std::string & id)` | 
`public virtual InstallTask::Ptr createInstallTask(`[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` | Creates a package installation task for the given pair.
`public virtual std::string installedPackageVersion(const std::string & id) const` | 
`public virtual `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` getLatestInstallableAsset(const `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options) const` | 
`public virtual bool hasAvailableUpdates(const `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair) const` | 
`public void clearCache()` | File Helper Methods.
`public bool clearPackageCache(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` & package)` | Clears a package archive from the local cache.
`public bool clearCacheFile(const std::string & fileName,bool whiny)` | Clears a file from the local cache.
`public bool hasCachedFile(`[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & asset)` | Checks if a package archive exists in the local cache.
`public bool isSupportedFileType(const std::string & fileName)` | Checks if the file type is a supported package archive.
`public std::string getCacheFilePath(const std::string & fileName)` | 
`public std::string getPackageDataDir(const std::string & id)` | 
`public virtual `[`Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)` & options()` | Accessors.
`public virtual `[`RemotePackageStore`](#classscy_1_1LiveCollection)` & remotePackages()` | 
`public virtual `[`LocalPackageStore`](#classscy_1_1LiveCollection)` & localPackages()` | 
`protected mutable std::mutex _mutex` | 
`protected `[`LocalPackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _localPackages` | 
`protected `[`RemotePackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _remotePackages` | 
`protected InstallTaskPtrVec _tasks` | 
`protected `[`Options`](./doc/api-pacm.md#structscy_1_1pacm_1_1PackageManager_1_1Options)` _options` | 
`protected void onPackageInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` | Callbacks.

## Members

#### `public `[`Signal](#classscy_1_1Signal)< void(const [http::Response`](#classscy_1_1http_1_1Response) &)`> RemotePackageResponse` 

Events.

Signals when the remote package list have been downloaded from the server.

#### `public `[`Signal](#classscy_1_1Signal)< void([LocalPackage`](#structscy_1_1pacm_1_1LocalPackage) &)`> PackageUninstalled` 

Signals when a package is uninstalled.



#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> InstallTaskCreated` 



Signals when an installation task is created, before it is started.

#### `public `[`Signal](#classscy_1_1Signal)< void(const [InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> InstallTaskComplete` 



Signals when a package installation tasks completes, either successfully or in error.

#### `public  PackageManager(const `[`Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)` & options)` 





#### `public virtual  ~PackageManager()` 





#### `public virtual void initialize()` 

Initialization Methods.



#### `public virtual void uninitialize()` 





#### `public virtual bool initialized() const` 





#### `public virtual void createDirectories()` 



Creates the package manager directory structure if it does not already exist.

#### `public virtual void queryRemotePackages()` 

Queries the server for a list of available packages.



#### `public virtual void loadLocalPackages()` 



Loads all local package manifests from file system. Clears all in memory package manifests.

#### `public virtual void loadLocalPackages(const std::string & dir)` 



Loads all local package manifests residing the the given directory. This method may be called multiple times for different paths because it does not clear in memory package manifests.

#### `public virtual bool saveLocalPackages(bool whiny)` 





#### `public virtual bool saveLocalPackage(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` & package,bool whiny)` 

Saves the local package manifest to the file system.



#### `public virtual void parseRemotePackages(const std::string & data)` 

Parse the remote packages from the given JSON data string.



#### `public virtual InstallTask::Ptr installPackage(const std::string & name,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` 

[Package](#structscy_1_1pacm_1_1Package) Installation Methods.

Installs a single package. The returned [InstallTask](#classscy_1_1pacm_1_1InstallTask) must be started. If the package is already up-to-date, a nullptr will be returned. Any other error will throw a std::runtime_error.

#### `public virtual bool installPackages(const StringVec & ids,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,`[`InstallMonitor`](#classscy_1_1pacm_1_1InstallMonitor)` * monitor,bool whiny)` 



Installs multiple packages. The same options will be passed to each task. If a [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor) instance was passed in the tasks will need to be started, otherwise they will be auto-started. The [PackageManager](#classscy_1_1pacm_1_1PackageManager) does not take ownership of the [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor).

#### `public virtual InstallTask::Ptr updatePackage(const std::string & name,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` 



Updates a single package. Throws an exception if the package does not exist. The returned [InstallTask](#classscy_1_1pacm_1_1InstallTask) must be started.

#### `public virtual bool updatePackages(const StringVec & ids,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,`[`InstallMonitor`](#classscy_1_1pacm_1_1InstallMonitor)` * monitor,bool whiny)` 



Updates multiple packages. Throws an exception if the package does not exist. If a [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor) instance was passed in the tasks will need to be started, otherwise they will be auto-started. The [PackageManager](#classscy_1_1pacm_1_1PackageManager) does not take ownership of the [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor).

#### `public virtual bool updateAllPackages(bool whiny)` 

Updates all installed packages.



#### `public virtual bool uninstallPackages(const StringVec & ids,bool whiny)` 

Uninstalls multiple packages.



#### `public virtual bool uninstallPackage(const std::string & id,bool whiny)` 

Uninstalls a single package.



#### `public virtual bool hasUnfinalizedPackages()` 



Returns true if there are updates available that have not yet been finalized. Packages may be unfinalized if there were files in use at the time of installation.

#### `public virtual bool finalizeInstallations(bool whiny)` 



Finalizes active installations by moving all package files to their target destination. If files are to be overwritten they must not be in use or finalization will fail.

#### `public virtual InstallTask::Ptr getInstallTask(const std::string & id) const` 

[Task](#classscy_1_1Task) Helper Methods.

Gets the install task for the given package ID.

#### `public virtual InstallTaskPtrVec tasks() const` 

Returns a list of all tasks.



#### `public virtual void cancelAllTasks()` 



Aborts all package installation tasks. All tasks must be aborted before clearing local or remote manifests.

#### `public virtual PackagePairVec getPackagePairs() const` 

[Package](#structscy_1_1pacm_1_1Package) Helper Methods.

Returns all package pairs, valid or invalid. Some pairs may not have both local and remote package pointers.

#### `public virtual PackagePairVec getUpdatablePackagePairs() const` 



Returns a list of package pairs which may be updated. All pairs will have both local and remote package pointers, and the remote version will be newer than the local version.

#### `public virtual `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` getPackagePair(const std::string & id,bool whiny) const` 



Returns a local and remote package pair. An exception will be thrown if either the local or remote packages aren't available or are invalid.

#### `public virtual `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` getOrCreatePackagePair(const std::string & id)` 



Returns a local and remote package pair. If the local package doesn't exist it will be created from the remote package. If the remote package doesn't exist a NotFoundException will be thrown.

#### `public virtual InstallTask::Ptr createInstallTask(`[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` 

Creates a package installation task for the given pair.



#### `public virtual std::string installedPackageVersion(const std::string & id) const` 



Returns the version number of an installed package. Exceptions will be thrown if the package does not exist, or is not fully installed.

#### `public virtual `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` getLatestInstallableAsset(const `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options) const` 



Returns the best asset to install, or throws a descriptive exception if no updates are available, or if the package is already up-to-date. This method takes version and SDK locks into consideration.

#### `public virtual bool hasAvailableUpdates(const `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair) const` 



Returns true if there are updates available for this package, false otherwise.

#### `public void clearCache()` 

File Helper Methods.

Clears all files in the cache directory.

#### `public bool clearPackageCache(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` & package)` 

Clears a package archive from the local cache.



#### `public bool clearCacheFile(const std::string & fileName,bool whiny)` 

Clears a file from the local cache.



#### `public bool hasCachedFile(`[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & asset)` 

Checks if a package archive exists in the local cache.



#### `public bool isSupportedFileType(const std::string & fileName)` 

Checks if the file type is a supported package archive.



#### `public std::string getCacheFilePath(const std::string & fileName)` 



Returns the full path of the cached file if it exists, or an empty path if the file doesn't exist.

#### `public std::string getPackageDataDir(const std::string & id)` 



Returns the package data directory for the given package ID.

#### `public virtual `[`Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)` & options()` 

Accessors.



#### `public virtual `[`RemotePackageStore`](#classscy_1_1LiveCollection)` & remotePackages()` 





#### `public virtual `[`LocalPackageStore`](#classscy_1_1LiveCollection)` & localPackages()` 





#### `protected mutable std::mutex _mutex` 





#### `protected `[`LocalPackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _localPackages` 





#### `protected `[`RemotePackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _remotePackages` 





#### `protected InstallTaskPtrVec _tasks` 





#### `protected `[`Options`](./doc/api-pacm.md#structscy_1_1pacm_1_1PackageManager_1_1Options)` _options` 





#### `protected void onPackageInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` 

Callbacks.



# struct `scy::pacm::InstallationState` 

```
struct scy::pacm::InstallationState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline std::string str(unsigned int id) const` | 

## Members

#### `public inline std::string str(unsigned int id) const` 





# struct `scy::pacm::InstallOptions` 


[Package](#structscy_1_1pacm_1_1Package) installation options.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string version` | If set then the given package version will be installed.
`public std::string sdkVersion` | 
`public std::string installDir` | 
`public inline  InstallOptions()` | 

## Members

#### `public std::string version` 

If set then the given package version will be installed.



#### `public std::string sdkVersion` 



If set then the latest package version for given SDK version will be installed.

#### `public std::string installDir` 



Install to the given location, otherwise the manager default `installDir` will be used.

#### `public inline  InstallOptions()` 





# struct `scy::pacm::LocalPackage` 

```
struct scy::pacm::LocalPackage
  : public scy::pacm::Package
```  



This class is a JSON representation of an installed local package that exists on the file system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LocalPackage()` | 
`public  LocalPackage(const json::value & src)` | 
`public  LocalPackage(const `[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` & src)` | 
`public virtual  ~LocalPackage()` | 
`public virtual void setState(const std::string & state)` | 
`public virtual void setInstallState(const std::string & state)` | 
`public virtual void setInstallDir(const std::string & dir)` | Set's the installation directory for this package.
`public virtual void setInstalledAsset(const `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & installedRemoteAsset)` | 
`public virtual void setVersion(const std::string & version)` | 
`public virtual void setVersionLock(const std::string & version)` | 
`public virtual void setSDKVersionLock(const std::string & version)` | 
`public virtual std::string version() const` | Returns the installed package version.
`public virtual std::string state() const` | Returns the current state of this package.
`public virtual std::string installState() const` | Returns the installation state of this package.
`public virtual std::string installDir() const` | Returns the installation directory for this package.
`public virtual std::string versionLock() const` | 
`public virtual std::string sdkLockedVersion() const` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` asset()` | 
`public virtual bool isInstalled() const` | 
`public virtual bool isFailed() const` | 
`public virtual `[`Manifest`](#structscy_1_1pacm_1_1LocalPackage_1_1Manifest)` manifest()` | Returns the installation manifest.
`public virtual bool verifyInstallManifest(bool allowEmpty)` | 
`public virtual std::string getInstalledFilePath(const std::string & fileName,bool whiny)` | 
`public virtual json::value & errors()` | 
`public virtual void addError(const std::string & message)` | 
`public virtual std::string lastError() const` | 
`public virtual void clearErrors()` | 
`public virtual bool valid() const` | 

## Members

#### `public  LocalPackage()` 





#### `public  LocalPackage(const json::value & src)` 





#### `public  LocalPackage(const `[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` & src)` 



Create the local package from the remote package reference with the following manipulations. 1) Add a local manifest element. 2) Remove asset mirror elements.

#### `public virtual  ~LocalPackage()` 





#### `public virtual void setState(const std::string & state)` 



Set's the overall package state. Possible values are: Installing, Installed, Failed, Uninstalled. If the packages completes while still Installing, this means the package has yet to be finalized.

#### `public virtual void setInstallState(const std::string & state)` 



Set's the package installation state. See [InstallationState](#structscy_1_1pacm_1_1InstallationState) for possible values.

#### `public virtual void setInstallDir(const std::string & dir)` 

Set's the installation directory for this package.



#### `public virtual void setInstalledAsset(const `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & installedRemoteAsset)` 



Sets the installed asset, once installed. This method also sets the version.

#### `public virtual void setVersion(const std::string & version)` 



Sets the current version of the local package. Installation must be complete.

#### `public virtual void setVersionLock(const std::string & version)` 



Locks the package at the given version. Once set this package will not be updated past the given version. Pass an empty string to remove the lock.

#### `public virtual void setSDKVersionLock(const std::string & version)` 



Locks the package at the given SDK version. Once set this package will only update to the most recent version with given SDK version. Pass an empty string to remove the lock.

#### `public virtual std::string version() const` 

Returns the installed package version.



#### `public virtual std::string state() const` 

Returns the current state of this package.



#### `public virtual std::string installState() const` 

Returns the installation state of this package.



#### `public virtual std::string installDir() const` 

Returns the installation directory for this package.



#### `public virtual std::string versionLock() const` 





#### `public virtual std::string sdkLockedVersion() const` 





#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` asset()` 



Returns the currently installed asset, if any. If none, the returned asset will be empty().

#### `public virtual bool isInstalled() const` 



Returns true or false depending on weather or not the package is installed successfully. False if package is in Failed state.

#### `public virtual bool isFailed() const` 





#### `public virtual `[`Manifest`](#structscy_1_1pacm_1_1LocalPackage_1_1Manifest)` manifest()` 

Returns the installation manifest.



#### `public virtual bool verifyInstallManifest(bool allowEmpty)` 





#### `public virtual std::string getInstalledFilePath(const std::string & fileName,bool whiny)` 



Returns the full full path of the installed file. Thrown an exception if the install directory is unset.

#### `public virtual json::value & errors()` 





#### `public virtual void addError(const std::string & message)` 





#### `public virtual std::string lastError() const` 





#### `public virtual void clearErrors()` 





#### `public virtual bool valid() const` 





# struct `scy::pacm::Package` 

```
struct scy::pacm::Package
  : public value
```  



This class is a JSON representation of an package belonging to the [PackageManager](#classscy_1_1pacm_1_1PackageManager).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Package()` | 
`public  Package(const json::value & src)` | 
`public virtual  ~Package()` | 
`public virtual std::string id() const` | 
`public virtual std::string name() const` | 
`public virtual std::string type() const` | 
`public virtual std::string author() const` | 
`public virtual std::string description() const` | 
`public virtual bool valid() const` | 
`public virtual void print(std::ostream & ost) const` | 

## Members

#### `public  Package()` 





#### `public  Package(const json::value & src)` 





#### `public virtual  ~Package()` 





#### `public virtual std::string id() const` 





#### `public virtual std::string name() const` 





#### `public virtual std::string type() const` 





#### `public virtual std::string author() const` 





#### `public virtual std::string description() const` 





#### `public virtual bool valid() const` 





#### `public virtual void print(std::ostream & ost) const` 





# struct `scy::pacm::PackagePair` 




This class provides pairing of a local and a remote package.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`LocalPackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1LocalPackage)` * local` | 
`public `[`RemotePackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1RemotePackage)` * remote` | 
`public  PackagePair(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local,`[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote)` | 
`public virtual bool valid() const` | 
`public std::string id() const` | 
`public std::string name() const` | 
`public std::string type() const` | 
`public std::string author() const` | 

## Members

#### `public `[`LocalPackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1LocalPackage)` * local` 



Returns true if there are no possible updates for this package, false otherwise.

#### `public `[`RemotePackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1RemotePackage)` * remote` 





#### `public  PackagePair(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local,`[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote)` 





#### `public virtual bool valid() const` 





#### `public std::string id() const` 





#### `public std::string name() const` 





#### `public std::string type() const` 





#### `public std::string author() const` 





# struct `scy::pacm::RemotePackage` 

```
struct scy::pacm::RemotePackage
  : public scy::pacm::Package
```  



This class is a JSON representation of an package existing on the remote server that may be downloaded and installed.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  RemotePackage()` | 
`public  RemotePackage(const json::value & src)` | 
`public virtual  ~RemotePackage()` | 
`public virtual json::value & assets()` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestAsset()` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` assetVersion(const std::string & version)` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestSDKAsset(const std::string & version)` | 

## Members

#### `public  RemotePackage()` 





#### `public  RemotePackage(const json::value & src)` 





#### `public virtual  ~RemotePackage()` 





#### `public virtual json::value & assets()` 





#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestAsset()` 



Returns the latest asset for this package. For local packages this is the currently installed version. For remote packages this is the latest available version. Throws an exception if no asset exists.

#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` assetVersion(const std::string & version)` 



Returns the latest asset for the given package version. Throws an exception if no asset exists.

#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestSDKAsset(const std::string & version)` 



Returns the latest asset for the given SDK version. This method is for safely installing plug-ins which must be compiled against a specific SDK version. The package JSON must have a "sdk-version" member for this function to work as intended. Throws an exception if no asset exists.

# struct `scy::pacm::Package::Asset` 




This class represents a archived file asset containing files belonging to the parent package.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public json::value & root` | 
`public  Asset(json::value & src)` | 
`public virtual  ~Asset()` | 
`public virtual std::string fileName() const` | 
`public virtual std::string version() const` | 
`public virtual std::string sdkVersion() const` | 
`public virtual std::string checksum() const` | 
`public virtual std::string url(int index) const` | 
`public virtual int fileSize() const` | 
`public virtual bool valid() const` | 
`public virtual void print(std::ostream & ost) const` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & operator=(const `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & r)` | 
`public virtual bool operator==(const `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & r) const` | 

## Members

#### `public json::value & root` 





#### `public  Asset(json::value & src)` 





#### `public virtual  ~Asset()` 





#### `public virtual std::string fileName() const` 





#### `public virtual std::string version() const` 





#### `public virtual std::string sdkVersion() const` 





#### `public virtual std::string checksum() const` 





#### `public virtual std::string url(int index) const` 





#### `public virtual int fileSize() const` 





#### `public virtual bool valid() const` 





#### `public virtual void print(std::ostream & ost) const` 





#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & operator=(const `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & r)` 





#### `public virtual bool operator==(const `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & r) const` 





# struct `scy::pacm::LocalPackage::Manifest` 




This class provides a list of all package files and their location on the file system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public json::value & root` | 
`public  Manifest(json::value & src)` | 
`public virtual  ~Manifest()` | 
`public virtual bool empty() const` | 
`public virtual void addFile(const std::string & path)` | 

## Members

#### `public json::value & root` 





#### `public  Manifest(json::value & src)` 





#### `public virtual  ~Manifest()` 





#### `public virtual bool empty() const` 





#### `public virtual void addFile(const std::string & path)` 





# struct `scy::pacm::PackageManager::Options` 


[Package](#structscy_1_1pacm_1_1Package) manager initialization options.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string endpoint` | The HTTP server endpoint.
`public std::string indexURI` | The HTTP server URI for querying packages JSON.
`public std::string httpUsername` | Username for HTTP basic auth.
`public std::string httpPassword` | PAssword for HTTP basic auth.
`public std::string httpOAuthToken` | Will be used instead of HTTP basic if provided.
`public std::string tempDir` | Directory where package files will be downloaded and extracted.
`public std::string dataDir` | Directory where package manifests will be kept.
`public std::string installDir` | Directory where packages will be installed.
`public std::string platform` | Platform (win32, linux, mac)
`public std::string checksumAlgorithm` | Checksum algorithm (MDS/SHA1)
`public bool clearFailedCache` | 
`public inline  Options(const std::string & root)` | 

## Members

#### `public std::string endpoint` 

The HTTP server endpoint.



#### `public std::string indexURI` 

The HTTP server URI for querying packages JSON.



#### `public std::string httpUsername` 

Username for HTTP basic auth.



#### `public std::string httpPassword` 

PAssword for HTTP basic auth.



#### `public std::string httpOAuthToken` 

Will be used instead of HTTP basic if provided.



#### `public std::string tempDir` 

Directory where package files will be downloaded and extracted.



#### `public std::string dataDir` 

Directory where package manifests will be kept.



#### `public std::string installDir` 

Directory where packages will be installed.



#### `public std::string platform` 

Platform (win32, linux, mac)



#### `public std::string checksumAlgorithm` 

Checksum algorithm (MDS/SHA1)



#### `public bool clearFailedCache` 



This flag tells the package manager weather or not to clear the package cache if installation fails.

#### `public inline  Options(const std::string & root)` 





