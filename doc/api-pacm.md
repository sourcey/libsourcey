# Module <!-- group --> `pacm`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::pacm`](#namespacescy_1_1pacm)    | 
`struct `[`scy::pacm::Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)    | 
`struct `[`scy::pacm::LocalPackage::Manifest`](#structscy_1_1pacm_1_1LocalPackage_1_1Manifest)    | 
`struct `[`scy::pacm::PackageManager::Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)    | [Package](#structscy_1_1pacm_1_1Package) manager initialization options.
# namespace `scy::pacm` {#namespacescy_1_1pacm}



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
# class `scy::pacm::InstallMonitor` {#classscy_1_1pacm_1_1InstallMonitor}






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
`protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` | 
`protected InstallTaskPtrVec _tasks` | 
`protected LocalPackageVec _packages` | 
`protected int _progress` | 
`protected virtual void onInstallStateChange(void * sender,`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` | 
`protected virtual void onInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` | 
`protected virtual void setProgress(int value)` | 

## Members

#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask](#classscy_1_1pacm_1_1InstallTask) &, const [InstallationState](#structscy_1_1pacm_1_1InstallationState) &, const [InstallationState`](#structscy_1_1pacm_1_1InstallationState) &)`> InstallStateChange` {#group__pacm_1ga5f7cb24385268ab5d2b50da72730d10a}

Proxies state change events from managed packages.



#### `public `[`Signal](#classscy_1_1Signal)< void([LocalPackage`](#structscy_1_1pacm_1_1LocalPackage) &)`> InstallComplete` {#group__pacm_1ga081f4bd707b531dfd4d38f2233588bf8}

Signals when a managed install task completes.



#### `public `[`Signal`](#classscy_1_1Signal)< void(int &)`> Progress` {#group__pacm_1ga293627c638d0d1f70e82546be766219a}

Signals on overall progress update [0-100].



#### `public `[`Signal`](#classscy_1_1Signal)< void(LocalPackageVec &)`> Complete` {#group__pacm_1ga9bdc4dad951bb031a861bb53efa411b4}

Signals on all tasks complete.



#### `public  InstallMonitor()` {#group__pacm_1ga0120805da5155e36bf6be738d58f1550}





#### `public virtual  ~InstallMonitor()` {#group__pacm_1ga9029008260c1d966e942f2b720d2237c}





#### `public virtual void addTask(InstallTask::Ptr task)` {#group__pacm_1gade82e15d697cebdcf4c27c79fba73d7b}

Adds a task to monitor.



#### `public virtual void startAll()` {#group__pacm_1gaa77ef7964ec12fd63329648d615588f7}

Starts all monitored tasks.



#### `public virtual void cancelAll()` {#group__pacm_1ga188395007e67e4562b350ebc945c347a}

Cancels all monitored tasks.



#### `public virtual bool isComplete() const` {#group__pacm_1ga7eb6c0cd1bcb8ed8b38451a38b93a818}



Returns true if all install tasks have completed, either successfully or unsuccessfully.

#### `public virtual InstallTaskPtrVec tasks() const` {#group__pacm_1ga25a15551d09c6b50c27a628f9f548ed6}

Returns the list of monitored package tasks.



#### `public virtual LocalPackageVec packages() const` {#group__pacm_1gae85037281be09f4c389ca0a1210d4793}

Returns the list of monitored packages.



#### `protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` {#group__pacm_1gadaa11ca20c7096e8a56feb1c48c7efff}





#### `protected InstallTaskPtrVec _tasks` {#group__pacm_1ga9466e58faabe5386ae0e8cc4b053baf0}





#### `protected LocalPackageVec _packages` {#group__pacm_1ga5c5266c23cb36a98bbaf513f3c58beab}





#### `protected int _progress` {#group__pacm_1ga3cfe9db2c67679216a9f84d31060c9c9}





#### `protected virtual void onInstallStateChange(void * sender,`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` {#group__pacm_1gaa8d8ae7796e966977d7ccc052e7e392e}





#### `protected virtual void onInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` {#group__pacm_1ga3fee590aa5c73808d0f36de1c70f5b0d}





#### `protected virtual void setProgress(int value)` {#group__pacm_1ga747222082b89e98e0d6befc96c1bcc16}





# class `scy::pacm::InstallTask` {#classscy_1_1pacm_1_1InstallTask}

```
class scy::pacm::InstallTask
  : public scy::async::Runnable
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
`public virtual bool cancelled() const` | True when the task has been cancelled.
`public virtual bool failed() const` | 
`public virtual bool success() const` | 
`public virtual bool complete() const` | 
`public virtual int progress() const` | 
`protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` | 
`protected `[`Idler`](./doc/api-base.md#classscy_1_1Idler)` _runner` | 
`protected `[`PackageManager`](./doc/api-pacm.md#classscy_1_1pacm_1_1PackageManager)` & _manager` | 
`protected `[`LocalPackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1LocalPackage)` * _local` | 
`protected `[`RemotePackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1RemotePackage)` * _remote` | 
`protected `[`InstallOptions`](./doc/api-pacm.md#structscy_1_1pacm_1_1InstallOptions)` _options` | 
`protected int _progress` | 
`protected bool _downloading` | 
`protected http::ClientConnection::Ptr _dlconn` | 
`protected uv::Loop * _loop` | 
`protected virtual void run()` | 
`protected virtual void onStateChange(`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` | 
`protected virtual void onDownloadProgress(const double & progress)` | 
`protected virtual void onDownloadComplete(const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` | 
`protected virtual void setProgress(int value)` | 

## Members

#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &, int &)`> Progress` {#group__pacm_1gaf703cf11a21b644acc7e59c1ebf26485}

Signals on progress update [0-100].



#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> Complete` {#group__pacm_1gaccf8591e8208940a4ad7c8133e3ec0ad}



Signals on task completion for both success and failure cases.

#### `public  InstallTask(`[`PackageManager`](#classscy_1_1pacm_1_1PackageManager)` & manager,`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local,`[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,uv::Loop * loop)` {#group__pacm_1gac517d62628a6206362e8f8b8e8b6889e}





#### `public virtual  ~InstallTask()` {#group__pacm_1ga136d640d67a5bab3c7a2c4a5607bd6db}





#### `public virtual void start()` {#group__pacm_1ga26a08fe537a7db788079445921650685}





#### `public virtual void cancel()` {#group__pacm_1ga8847a1e10771443fa49ea35939a22567}





#### `public virtual void doDownload()` {#group__pacm_1gaf815fbda71ebf69e76e015b68991e7b4}

Downloads the package archive from the server.



#### `public virtual void doExtract()` {#group__pacm_1ga2f72241766459b160c804adc1478f5de}



Extracts the downloaded package files to the intermediate directory.

#### `public virtual void doFinalize()` {#group__pacm_1ga6a1835d063d88174640ef5a28f883ac8}



Moves extracted files from the intermediate directory to the installation directory.

#### `public virtual void setComplete()` {#group__pacm_1ga26d9a2a7502b8b85c7ee3a925567d882}



Called when the task completes either successfully or in error. This will trigger destruction.

#### `public virtual `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` getRemoteAsset() const` {#group__pacm_1gac019583f25b3acdb49d9341c1a7978c6}





#### `public virtual `[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local() const` {#group__pacm_1gadbcbd1e53c273436d7115a41ed1873f7}





#### `public virtual `[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote() const` {#group__pacm_1gaf63f6840681ee74ee5664c82ffb889a2}





#### `public virtual `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options()` {#group__pacm_1gadf2774d8799a8fd865fc4f6b73a00ed1}





#### `public virtual uv::Loop * loop() const` {#group__pacm_1ga682298509ac94f66851b7760ce672c57}





#### `public virtual bool valid() const` {#group__pacm_1ga0a49aa89c7ace046190a1920c8db2704}





#### `public virtual bool cancelled() const` {#group__pacm_1gad4433a04ac1bb9dd5a6d2f222df77418}

True when the task has been cancelled.



#### `public virtual bool failed() const` {#group__pacm_1ga98d6a383a58aab12cf84e2591774571f}





#### `public virtual bool success() const` {#group__pacm_1ga10c6401c60f22f2845c9560e58496907}





#### `public virtual bool complete() const` {#group__pacm_1ga0dab536f7d0d8eddcb0cc5c9bddd1a87}





#### `public virtual int progress() const` {#group__pacm_1ga3dbca4f9b48fa5266333f7428e45338b}





#### `protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` {#group__pacm_1ga55b4143cc2cd3c86d140fbd4152ff264}





#### `protected `[`Idler`](./doc/api-base.md#classscy_1_1Idler)` _runner` {#group__pacm_1gaf178666e39b153b95d05945e47602099}





#### `protected `[`PackageManager`](./doc/api-pacm.md#classscy_1_1pacm_1_1PackageManager)` & _manager` {#group__pacm_1ga75bf291aa5b026cc178ba1361d44ec92}





#### `protected `[`LocalPackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1LocalPackage)` * _local` {#group__pacm_1gaed93e1d4951c6672b7f2ce7a91469cfd}





#### `protected `[`RemotePackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1RemotePackage)` * _remote` {#group__pacm_1ga39c422153b97c6aa95a1b2fe74a4c0b5}





#### `protected `[`InstallOptions`](./doc/api-pacm.md#structscy_1_1pacm_1_1InstallOptions)` _options` {#group__pacm_1ga3d6b1dbf5dc37e7aaf8d012f37d2c2cf}





#### `protected int _progress` {#group__pacm_1ga969d6a197cb58d27f056a989d413057d}





#### `protected bool _downloading` {#group__pacm_1gac770bceb041aaa041c79f05a03a290b9}





#### `protected http::ClientConnection::Ptr _dlconn` {#group__pacm_1gaa286e79832b53a9a13ff50c6db6166d3}





#### `protected uv::Loop * _loop` {#group__pacm_1gab2e5dc630f6211673263d278d29be932}





#### `protected virtual void run()` {#group__pacm_1gab2791d8750f5bf0deece4e7fd708a0c9}



Called asynchronously by the thread to do the work.

#### `protected virtual void onStateChange(`[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & state,const `[`InstallationState`](#structscy_1_1pacm_1_1InstallationState)` & oldState)` {#group__pacm_1ga2f29805629456fceb726bd8c7f03c253}





#### `protected virtual void onDownloadProgress(const double & progress)` {#group__pacm_1ga5a9f8befa6682fc5b725bab29d9cf60f}





#### `protected virtual void onDownloadComplete(const `[`http::Response`](#classscy_1_1http_1_1Response)` & response)` {#group__pacm_1ga9c58cacb35a5d15d143c329b404e2b76}





#### `protected virtual void setProgress(int value)` {#group__pacm_1gaaa96e157962a5c8bfb70a84f5a49d634}





# class `scy::pacm::PackageManager` {#classscy_1_1pacm_1_1PackageManager}




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
`protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` | 
`protected `[`LocalPackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _localPackages` | 
`protected `[`RemotePackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _remotePackages` | 
`protected InstallTaskPtrVec _tasks` | 
`protected `[`Options`](./doc/api-pacm.md#structscy_1_1pacm_1_1PackageManager_1_1Options)` _options` | 
`protected void onPackageInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` | Callbacks.

## Members

#### `public `[`Signal](#classscy_1_1Signal)< void(const [http::Response`](#classscy_1_1http_1_1Response) &)`> RemotePackageResponse` {#group__pacm_1gac9bfa675fc261bd38052ae17bae25356}

Events.

Signals when the remote package list have been downloaded from the server.

#### `public `[`Signal](#classscy_1_1Signal)< void([LocalPackage`](#structscy_1_1pacm_1_1LocalPackage) &)`> PackageUninstalled` {#group__pacm_1gab05d4bd29a2c2ecc7579e58f919d43c5}

Signals when a package is uninstalled.



#### `public `[`Signal](#classscy_1_1Signal)< void([InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> InstallTaskCreated` {#group__pacm_1ga7fbc7057386ad4ec58af8223d56c7bef}



Signals when an installation task is created, before it is started.

#### `public `[`Signal](#classscy_1_1Signal)< void(const [InstallTask`](#classscy_1_1pacm_1_1InstallTask) &)`> InstallTaskComplete` {#group__pacm_1ga4e0d38796a84080bbf09756a553503c7}



Signals when a package installation tasks completes, either successfully or in error.

#### `public  PackageManager(const `[`Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)` & options)` {#group__pacm_1ga0294b3732c8c91eae3066bf22088dd8f}





#### `public virtual  ~PackageManager()` {#group__pacm_1ga0cc70a4263385dff8736a9e93d1e8e91}





#### `public virtual void initialize()` {#group__pacm_1gad2aa7211618560cec5cadf2d2b935e3f}

Initialization Methods.



#### `public virtual void uninitialize()` {#group__pacm_1ga3f0734d6e0baf127459b596be8f69041}





#### `public virtual bool initialized() const` {#group__pacm_1ga35d5d38a71defe859770d1f5ea45dc29}





#### `public virtual void createDirectories()` {#group__pacm_1ga7eac61590241621a6945610c642bbd20}



Creates the package manager directory structure if it does not already exist.

#### `public virtual void queryRemotePackages()` {#group__pacm_1ga7a6a87813079f346cf8b1248ef6f954e}

Queries the server for a list of available packages.



#### `public virtual void loadLocalPackages()` {#group__pacm_1ga274d4bdc0fb2824165912e29eaa4a83a}



Loads all local package manifests from file system. Clears all in memory package manifests.

#### `public virtual void loadLocalPackages(const std::string & dir)` {#group__pacm_1gac57d2e0df47bd611aadd0018a8562bf5}



Loads all local package manifests residing the the given directory. This method may be called multiple times for different paths because it does not clear in memory package manifests.

#### `public virtual bool saveLocalPackages(bool whiny)` {#group__pacm_1ga0dd3c90a7e9f85b20e6c0e44484be0af}





#### `public virtual bool saveLocalPackage(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` & package,bool whiny)` {#group__pacm_1gaf7b9979bcb8cb9cb83f7ad1cb435879c}

Saves the local package manifest to the file system.



#### `public virtual void parseRemotePackages(const std::string & data)` {#group__pacm_1ga5a84f69090718da9af13247e64e28156}

Parse the remote packages from the given JSON data string.



#### `public virtual InstallTask::Ptr installPackage(const std::string & name,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` {#group__pacm_1gad295633f0783292623a120b227685090}

[Package](#structscy_1_1pacm_1_1Package) Installation Methods.

Installs a single package. The returned [InstallTask](#classscy_1_1pacm_1_1InstallTask) must be started. If the package is already up-to-date, a nullptr will be returned. Any other error will throw a std::runtime_error.

#### `public virtual bool installPackages(const StringVec & ids,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,`[`InstallMonitor`](#classscy_1_1pacm_1_1InstallMonitor)` * monitor,bool whiny)` {#group__pacm_1ga606a44cfed11394c05485adc351e9c04}



Installs multiple packages. The same options will be passed to each task. If a [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor) instance was passed in the tasks will need to be started, otherwise they will be auto-started. The [PackageManager](#classscy_1_1pacm_1_1PackageManager) does not take ownership of the [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor).

#### `public virtual InstallTask::Ptr updatePackage(const std::string & name,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` {#group__pacm_1ga027124b092208da323cfc62299d9d4bc}



Updates a single package. Throws an exception if the package does not exist. The returned [InstallTask](#classscy_1_1pacm_1_1InstallTask) must be started.

#### `public virtual bool updatePackages(const StringVec & ids,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options,`[`InstallMonitor`](#classscy_1_1pacm_1_1InstallMonitor)` * monitor,bool whiny)` {#group__pacm_1gac48da691471662ce57f5815ac9c91ab0}



Updates multiple packages. Throws an exception if the package does not exist. If a [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor) instance was passed in the tasks will need to be started, otherwise they will be auto-started. The [PackageManager](#classscy_1_1pacm_1_1PackageManager) does not take ownership of the [InstallMonitor](#classscy_1_1pacm_1_1InstallMonitor).

#### `public virtual bool updateAllPackages(bool whiny)` {#group__pacm_1gaabc1315c35c2204028ef39ef77201d33}

Updates all installed packages.



#### `public virtual bool uninstallPackages(const StringVec & ids,bool whiny)` {#group__pacm_1ga21634eceac90c02a342e413eb444a9d1}

Uninstalls multiple packages.



#### `public virtual bool uninstallPackage(const std::string & id,bool whiny)` {#group__pacm_1ga06e4a2f271745f6ea4c50d4a85953bd5}

Uninstalls a single package.



#### `public virtual bool hasUnfinalizedPackages()` {#group__pacm_1ga736cd4d51f8c5e39dcfa2159fcfa0c8e}



Returns true if there are updates available that have not yet been finalized. Packages may be unfinalized if there were files in use at the time of installation.

#### `public virtual bool finalizeInstallations(bool whiny)` {#group__pacm_1ga65f16318834b79ddfa651fbbee66b301}



Finalizes active installations by moving all package files to their target destination. If files are to be overwritten they must not be in use or finalization will fail.

#### `public virtual InstallTask::Ptr getInstallTask(const std::string & id) const` {#group__pacm_1gad2d2c89ce41a9e620bdb0abfb005c369}

[Task](#classscy_1_1Task) Helper Methods.

Gets the install task for the given package ID.

#### `public virtual InstallTaskPtrVec tasks() const` {#group__pacm_1ga06761269e2391f30c5e0564156af8374}

Returns a list of all tasks.



#### `public virtual void cancelAllTasks()` {#group__pacm_1ga86a2bcebdc5c0bd217877ff180f1640b}



Aborts all package installation tasks. All tasks must be aborted before clearing local or remote manifests.

#### `public virtual PackagePairVec getPackagePairs() const` {#group__pacm_1ga11c411f6e676fb723561f8dc0c7bab4c}

[Package](#structscy_1_1pacm_1_1Package) Helper Methods.

Returns all package pairs, valid or invalid. Some pairs may not have both local and remote package pointers.

#### `public virtual PackagePairVec getUpdatablePackagePairs() const` {#group__pacm_1ga8d2d399c321a02b82363387c25641aa0}



Returns a list of package pairs which may be updated. All pairs will have both local and remote package pointers, and the remote version will be newer than the local version.

#### `public virtual `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` getPackagePair(const std::string & id,bool whiny) const` {#group__pacm_1ga2b2ef4158487fa1d82350994b5af38a9}



Returns a local and remote package pair. An exception will be thrown if either the local or remote packages aren't available or are invalid.

#### `public virtual `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` getOrCreatePackagePair(const std::string & id)` {#group__pacm_1gada7825aae6e2a54ee37332750686f67b}



Returns a local and remote package pair. If the local package doesn't exist it will be created from the remote package. If the remote package doesn't exist a NotFoundException will be thrown.

#### `public virtual InstallTask::Ptr createInstallTask(`[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options)` {#group__pacm_1gafa07546ae7c88588169868deda5401ac}

Creates a package installation task for the given pair.



#### `public virtual std::string installedPackageVersion(const std::string & id) const` {#group__pacm_1ga14871be8a910f944116f75c379352803}



Returns the version number of an installed package. Exceptions will be thrown if the package does not exist, or is not fully installed.

#### `public virtual `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` getLatestInstallableAsset(const `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair,const `[`InstallOptions`](#structscy_1_1pacm_1_1InstallOptions)` & options) const` {#group__pacm_1ga5b0b61d38eb166ee35ae458d45bc3ea2}



Returns the best asset to install, or throws a descriptive exception if no updates are available, or if the package is already up-to-date. This method takes version and SDK locks into consideration.

#### `public virtual bool hasAvailableUpdates(const `[`PackagePair`](#structscy_1_1pacm_1_1PackagePair)` & pair) const` {#group__pacm_1ga994577213392b7452e0cc4f49893295a}



Returns true if there are updates available for this package, false otherwise.

#### `public void clearCache()` {#group__pacm_1gae83f81efc6fac83c6b1feff2ab5dcc9f}

File Helper Methods.

Clears all files in the cache directory.

#### `public bool clearPackageCache(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` & package)` {#group__pacm_1gad57a7658f320091a47c9f592d34daf7f}

Clears a package archive from the local cache.



#### `public bool clearCacheFile(const std::string & fileName,bool whiny)` {#group__pacm_1ga547f44c7bb1c2307ac62b7a73aa9cb72}

Clears a file from the local cache.



#### `public bool hasCachedFile(`[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & asset)` {#group__pacm_1gabba22fcdb14b4ffe8c1bbaaed0845bf8}

Checks if a package archive exists in the local cache.



#### `public bool isSupportedFileType(const std::string & fileName)` {#group__pacm_1ga2b6d7aca72d07ffb066ed33126e33b00}

Checks if the file type is a supported package archive.



#### `public std::string getCacheFilePath(const std::string & fileName)` {#group__pacm_1gaf98ef2e87f1508708c8d6387a4442fc2}



Returns the full path of the cached file if it exists, or an empty path if the file doesn't exist.

#### `public std::string getPackageDataDir(const std::string & id)` {#group__pacm_1ga0e449180c70179434ff03226f8f612ca}



Returns the package data directory for the given package ID.

#### `public virtual `[`Options`](#structscy_1_1pacm_1_1PackageManager_1_1Options)` & options()` {#group__pacm_1ga2f058814ce88bd8f906fd1a0798ee3dd}

Accessors.



#### `public virtual `[`RemotePackageStore`](#classscy_1_1LiveCollection)` & remotePackages()` {#group__pacm_1ga666c75b4746425e5dd7c0dbbe52062ed}





#### `public virtual `[`LocalPackageStore`](#classscy_1_1LiveCollection)` & localPackages()` {#group__pacm_1ga632003da9cc5e262bdbc679ee4dd2c9a}





#### `protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` {#group__pacm_1ga90184aa68ff6cb316279af53a1eae05a}





#### `protected `[`LocalPackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _localPackages` {#group__pacm_1ga1f7e142696d6a5cace81ff92036d5079}





#### `protected `[`RemotePackageStore`](./doc/api-base.md#classscy_1_1LiveCollection)` _remotePackages` {#group__pacm_1ga8cc192fdcf6a3aafd15a9083504e37da}





#### `protected InstallTaskPtrVec _tasks` {#group__pacm_1ga86909426338d228d0a01de9a98332617}





#### `protected `[`Options`](./doc/api-pacm.md#structscy_1_1pacm_1_1PackageManager_1_1Options)` _options` {#group__pacm_1ga6883f4d86ccd9d91baf44e737b4e3913}





#### `protected void onPackageInstallComplete(`[`InstallTask`](#classscy_1_1pacm_1_1InstallTask)` & task)` {#group__pacm_1ga51dc83f2084498013b72c545d9ec5124}

Callbacks.



# struct `scy::pacm::InstallationState` {#structscy_1_1pacm_1_1InstallationState}

```
struct scy::pacm::InstallationState
  : public scy::State
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline virtual std::string str(unsigned int id) const` | 

## Members

#### `public inline virtual std::string str(unsigned int id) const` {#group__pacm_1gaa40b0c6a943b11ea07aa7464d60804d2}





# struct `scy::pacm::InstallOptions` {#structscy_1_1pacm_1_1InstallOptions}


[Package](#structscy_1_1pacm_1_1Package) installation options.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string version` | If set then the given package version will be installed.
`public std::string sdkVersion` | 
`public std::string installDir` | 
`public inline  InstallOptions()` | 

## Members

#### `public std::string version` {#group__pacm_1ga9042e51b870e079ca083c96bedf53a49}

If set then the given package version will be installed.



#### `public std::string sdkVersion` {#group__pacm_1ga179a2d6514580071b30f8b4f87b77b3a}



If set then the latest package version for given SDK version will be installed.

#### `public std::string installDir` {#group__pacm_1gaa7c2627c783fa1ef3b919c40c8c51acb}



Install to the given location, otherwise the manager default installDir will be used.

#### `public inline  InstallOptions()` {#group__pacm_1ga948d1e6960c54926cf870ac7e6aef7ab}





# struct `scy::pacm::LocalPackage` {#structscy_1_1pacm_1_1LocalPackage}

```
struct scy::pacm::LocalPackage
  : public scy::pacm::Package
```  



This class is a JSON representation of an installed local package that exists on the file system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  LocalPackage()` | 
`public  LocalPackage(const json::Value & src)` | 
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
`public virtual json::Value & errors()` | 
`public virtual void addError(const std::string & message)` | 
`public virtual std::string lastError() const` | 
`public virtual void clearErrors()` | 
`public virtual bool valid() const` | 

## Members

#### `public  LocalPackage()` {#group__pacm_1ga146358d8ea6e0742af85e8dd1ed3cfbe}





#### `public  LocalPackage(const json::Value & src)` {#group__pacm_1ga63ff42ef32362320b2a22bdaf03a325e}





#### `public  LocalPackage(const `[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` & src)` {#group__pacm_1ga8d915798d4874783e5f1a93460c8a790}



Create the local package from the remote package reference with the following manipulations. 1) Add a local manifest element. 2) Remove asset mirror elements.

#### `public virtual  ~LocalPackage()` {#group__pacm_1gae753b7b0b571a560586e8f9f5f04c22b}





#### `public virtual void setState(const std::string & state)` {#group__pacm_1gad38e4481bbac043f2bbcd7dd0cf23c15}



Set's the overall package state. Possible values are: Installing, Installed, Failed, Uninstalled. If the packages completes while still Installing, this means the package has yet to be finalized.

#### `public virtual void setInstallState(const std::string & state)` {#group__pacm_1ga43dfd9e6481e0d3f7e42767024db6051}



Set's the package installation state. See [InstallationState](#structscy_1_1pacm_1_1InstallationState) for possible values.

#### `public virtual void setInstallDir(const std::string & dir)` {#group__pacm_1gad2f95cca3661df99a22d600cda6cf73d}

Set's the installation directory for this package.



#### `public virtual void setInstalledAsset(const `[`Package::Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & installedRemoteAsset)` {#group__pacm_1ga2d16fefb83a30029394f13655f82999f}



Sets the installed asset, once installed. This method also sets the version.

#### `public virtual void setVersion(const std::string & version)` {#group__pacm_1ga1fe540ab674a6df4f1fe21beba27d0bb}



Sets the current version of the local package. Installation must be complete.

#### `public virtual void setVersionLock(const std::string & version)` {#group__pacm_1ga00f12d1314c1fe28733935ee118574f9}



Locks the package at the given version. Once set this package will not be updated past the given version. Pass an empty string to remove the lock.

#### `public virtual void setSDKVersionLock(const std::string & version)` {#group__pacm_1gadf546ce2b38e0afc4efddbf701fb573f}



Locks the package at the given SDK version. Once set this package will only update to the most recent version with given SDK version. Pass an empty string to remove the lock.

#### `public virtual std::string version() const` {#group__pacm_1gad30980d365279cfaee755cfe0561a170}

Returns the installed package version.



#### `public virtual std::string state() const` {#group__pacm_1ga4c5569e8655f1994146458e6f2cb13cf}

Returns the current state of this package.



#### `public virtual std::string installState() const` {#group__pacm_1ga5d9b98d55946300cd06145855241b13d}

Returns the installation state of this package.



#### `public virtual std::string installDir() const` {#group__pacm_1ga7f84ea33e777a251e2893a1f69fa31e3}

Returns the installation directory for this package.



#### `public virtual std::string versionLock() const` {#group__pacm_1ga750d04ba34e63bca0c4c5761f161cea9}





#### `public virtual std::string sdkLockedVersion() const` {#group__pacm_1ga0e32d29a0d4c5ac84f85e81393ed003f}





#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` asset()` {#group__pacm_1ga8bf8741c74d2cc37fa1e6731bebbf37f}



Returns the currently installed asset, if any. If none, the returned asset will be empty().

#### `public virtual bool isInstalled() const` {#group__pacm_1gac915b269b5ee73baf22c81a44a337e9d}



Returns true or false depending on weather or not the package is installed successfully. False if package is in Failed state.

#### `public virtual bool isFailed() const` {#group__pacm_1ga7f0274050c627c6a040ff65cb75aa4ec}





#### `public virtual `[`Manifest`](#structscy_1_1pacm_1_1LocalPackage_1_1Manifest)` manifest()` {#group__pacm_1ga9ce8a07f05febda5e87b1a2a706af857}

Returns the installation manifest.



#### `public virtual bool verifyInstallManifest(bool allowEmpty)` {#group__pacm_1gaf1a9c2bdd76d940f200c53e6e175a587}





#### `public virtual std::string getInstalledFilePath(const std::string & fileName,bool whiny)` {#group__pacm_1ga9f24a8d655179921947edd97ff2a4395}



Returns the full full path of the installed file. Thrown an exception if the install directory is unset.

#### `public virtual json::Value & errors()` {#group__pacm_1gafd42be4bfdea014abf73ab0ceb06aa23}





#### `public virtual void addError(const std::string & message)` {#group__pacm_1ga359b1844b6d513f8e7daa83ce2eb1467}





#### `public virtual std::string lastError() const` {#group__pacm_1gae1e11871cc82282a8ad592ba55fdf121}





#### `public virtual void clearErrors()` {#group__pacm_1gafb51214393066dc3a75690fa5fff966e}





#### `public virtual bool valid() const` {#group__pacm_1ga6778e88ac7cb3bdc1fba171e9cfe4e85}





# struct `scy::pacm::Package` {#structscy_1_1pacm_1_1Package}

```
struct scy::pacm::Package
  : public Value
```  



This class is a JSON representation of an package belonging to the [PackageManager](#classscy_1_1pacm_1_1PackageManager).

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Package()` | 
`public  Package(const json::Value & src)` | 
`public virtual  ~Package()` | 
`public virtual std::string id() const` | 
`public virtual std::string name() const` | 
`public virtual std::string type() const` | 
`public virtual std::string author() const` | 
`public virtual std::string description() const` | 
`public virtual bool valid() const` | 
`public virtual void print(std::ostream & ost) const` | 

## Members

#### `public  Package()` {#group__pacm_1ga2703eff38205e8c4fc502029c6666c31}





#### `public  Package(const json::Value & src)` {#group__pacm_1ga044b5f3f5a635f43f7df6458ad1dc2e9}





#### `public virtual  ~Package()` {#group__pacm_1ga33db4f8d2056f384b0c9797fa3ae35c0}





#### `public virtual std::string id() const` {#group__pacm_1gadf03f04052d025ba84db73312dea23bb}





#### `public virtual std::string name() const` {#group__pacm_1ga7fc35ed22e76fe0e4dc8e42ed82e5f3c}





#### `public virtual std::string type() const` {#group__pacm_1gaf6e00584f5cc5ed044cd28e55f87ccea}





#### `public virtual std::string author() const` {#group__pacm_1gad4b921e4c2f96f90b9762edfec3ad752}





#### `public virtual std::string description() const` {#group__pacm_1ga6466be90a103884930f146af5acdc1a2}





#### `public virtual bool valid() const` {#group__pacm_1gab539f78ec4bc6fceb3944dc34936ec6c}





#### `public virtual void print(std::ostream & ost) const` {#group__pacm_1gaa2fc0fb484ee99c87f2997b3f8a3fe6b}





# struct `scy::pacm::PackagePair` {#structscy_1_1pacm_1_1PackagePair}




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

#### `public `[`LocalPackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1LocalPackage)` * local` {#group__pacm_1gadbaaae13eaaf203a08d8321bf0ca9bb0}



Returns true if there are no possible updates for this package, false otherwise.

#### `public `[`RemotePackage`](./doc/api-pacm.md#structscy_1_1pacm_1_1RemotePackage)` * remote` {#group__pacm_1ga64dacf8c8eafba27426e2f9a85cbd4b7}





#### `public  PackagePair(`[`LocalPackage`](#structscy_1_1pacm_1_1LocalPackage)` * local,`[`RemotePackage`](#structscy_1_1pacm_1_1RemotePackage)` * remote)` {#group__pacm_1ga0cfb58ccd49a5bff96f9c148ff7d39ef}





#### `public virtual bool valid() const` {#group__pacm_1gaff5ec132e15164c53c6874e690fb4c6c}





#### `public std::string id() const` {#group__pacm_1ga8bf565f8d76464a920733d01c68d9e78}





#### `public std::string name() const` {#group__pacm_1ga19214f72c4a06d16587f9ca8d4fc5f9b}





#### `public std::string type() const` {#group__pacm_1ga6ea1f67837c2ac9c11b46826b3eb79d6}





#### `public std::string author() const` {#group__pacm_1ga53e2b423f63a656def7af02faa7ef00b}





# struct `scy::pacm::RemotePackage` {#structscy_1_1pacm_1_1RemotePackage}

```
struct scy::pacm::RemotePackage
  : public scy::pacm::Package
```  



This class is a JSON representation of an package existing on the remote server that may be downloaded and installed.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  RemotePackage()` | 
`public  RemotePackage(const json::Value & src)` | 
`public virtual  ~RemotePackage()` | 
`public virtual json::Value & assets()` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestAsset()` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` assetVersion(const std::string & version)` | 
`public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestSDKAsset(const std::string & version)` | 

## Members

#### `public  RemotePackage()` {#group__pacm_1gaf64c095ee7ff79f3b3222c1e31e3f9b9}





#### `public  RemotePackage(const json::Value & src)` {#group__pacm_1ga494ab4ddc913b5852fccc89f64c4d0c0}





#### `public virtual  ~RemotePackage()` {#group__pacm_1ga40701168173d0770c14a9ab7cd48c5c6}





#### `public virtual json::Value & assets()` {#group__pacm_1ga728fa1d03afd6f6d6a35ac53985e599d}





#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestAsset()` {#group__pacm_1ga78a9d3c1e3023826f3c39f701cbf76cd}



Returns the latest asset for this package. For local packages this is the currently installed version. For remote packages this is the latest available version. Throws an exception if no asset exists.

#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` assetVersion(const std::string & version)` {#group__pacm_1ga2aecae77982bef0703bd05e3381a4396}



Returns the latest asset for the given package version. Throws an exception if no asset exists.

#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` latestSDKAsset(const std::string & version)` {#group__pacm_1ga43652748df730786052a32cfbc212971}



Returns the latest asset for the given SDK version. This method is for safely installing plug-ins which must be compiled against a specific SDK version. The package JSON must have a "sdk-version" member for this function to work as intended. Throws an exception if no asset exists.

# struct `scy::pacm::Package::Asset` {#structscy_1_1pacm_1_1Package_1_1Asset}




This class represents a archived file asset containing files belonging to the parent package.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public json::Value & root` | 
`public  Asset(json::Value & src)` | 
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

#### `public json::Value & root` {#group__pacm_1ga259b6a7ae9ef17c65cafe104161f951f}





#### `public  Asset(json::Value & src)` {#group__pacm_1ga83de2a74e72e53964fdfeba0868b926d}





#### `public virtual  ~Asset()` {#group__pacm_1ga9555259d3bcd6750e24f1c5cef5bd441}





#### `public virtual std::string fileName() const` {#group__pacm_1ga9a79ee260434ba1b30489d1d6e9dd421}





#### `public virtual std::string version() const` {#group__pacm_1ga6f83300447f0b5c1f7c4ec43cba3a5da}





#### `public virtual std::string sdkVersion() const` {#group__pacm_1gabed743e99c221cc89bc4531db63b1e38}





#### `public virtual std::string checksum() const` {#group__pacm_1gab4c084cccb50ec5c9597cf441cfe5a39}





#### `public virtual std::string url(int index) const` {#group__pacm_1gafbad31de39ae32ca71a513142d4b6ded}





#### `public virtual int fileSize() const` {#group__pacm_1ga6a460e9ddb8c78f258a5ad16f6ef07e6}





#### `public virtual bool valid() const` {#group__pacm_1ga20d7328f063017efa5e8b5221361a51b}





#### `public virtual void print(std::ostream & ost) const` {#group__pacm_1ga3d847d7913a955228cf0a4600e8e9452}





#### `public virtual `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & operator=(const `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & r)` {#group__pacm_1gae5c60ffe2dd0ff7459182a5ba32ff8f8}





#### `public virtual bool operator==(const `[`Asset`](#structscy_1_1pacm_1_1Package_1_1Asset)` & r) const` {#group__pacm_1ga11abf75c67f982309036a02d79d3084b}





# struct `scy::pacm::LocalPackage::Manifest` {#structscy_1_1pacm_1_1LocalPackage_1_1Manifest}




This class provides a list of all package files and their location on the file system.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public json::Value & root` | 
`public  Manifest(json::Value & src)` | 
`public virtual  ~Manifest()` | 
`public virtual bool empty() const` | 
`public virtual void addFile(const std::string & path)` | 

## Members

#### `public json::Value & root` {#group__pacm_1ga7a2c9aa2955ced26646680f538ef6140}





#### `public  Manifest(json::Value & src)` {#group__pacm_1ga22abf418916161577ca5c3e5614059a8}





#### `public virtual  ~Manifest()` {#group__pacm_1ga9d7098ebc7d12fc1dbabd8c2d07aa05b}





#### `public virtual bool empty() const` {#group__pacm_1gaa767187173b460a6ed903ad7b5237b87}





#### `public virtual void addFile(const std::string & path)` {#group__pacm_1ga46bf1459f6d73eb846287739d0b0c2f2}





# struct `scy::pacm::PackageManager::Options` {#structscy_1_1pacm_1_1PackageManager_1_1Options}


[Package](#structscy_1_1pacm_1_1Package) manager initialization options.



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string endpoint` | The HTTP server endpoint.
`public std::string indexURI` | The HTTP server URI for querying packages JSON.
`public std::string httpUsername` | Username for HTTP basic auth.
`public std::string httpPassword` | PAssword for HTTP basic auth.
`public std::string httpOAuthToken` | Will be used instead of HTTP basic if provided.
`public std::string tempDir` | 
`public std::string dataDir` | Directory where package manifests will be kept.
`public std::string installDir` | Directory where packages will be installed.
`public std::string platform` | Platform (win32, linux, mac)
`public std::string checksumAlgorithm` | Checksum algorithm (MDS/SHA1)
`public bool clearFailedCache` | to clear the package cache if installation fails.
`public inline  Options(const std::string & root)` | 

## Members

#### `public std::string endpoint` {#group__pacm_1ga17eaf31ffc1fc663c74715c0861feaa7}

The HTTP server endpoint.



#### `public std::string indexURI` {#group__pacm_1gaeaf07e705ed06b76a6e6da069b06933a}

The HTTP server URI for querying packages JSON.



#### `public std::string httpUsername` {#group__pacm_1ga2d4089adeb8ea1b332289aa912f9670d}

Username for HTTP basic auth.



#### `public std::string httpPassword` {#group__pacm_1gad72b21a0fd0afc7d0afbe79a267977c9}

PAssword for HTTP basic auth.



#### `public std::string httpOAuthToken` {#group__pacm_1ga3ae63176c907fe93eafb52451c55aa5d}

Will be used instead of HTTP basic if provided.



#### `public std::string tempDir` {#group__pacm_1ga9baae9668ea2afb09d479bc4e1622059}



Directory where package files will be downloaded and extracted

#### `public std::string dataDir` {#group__pacm_1ga2983a9e79dab4c044b56a53c3bec3ba0}

Directory where package manifests will be kept.



#### `public std::string installDir` {#group__pacm_1ga18001156609c5fe9845f4d0c259964ff}

Directory where packages will be installed.



#### `public std::string platform` {#group__pacm_1gaa9e36a87975600551306e8551065414b}

Platform (win32, linux, mac)



#### `public std::string checksumAlgorithm` {#group__pacm_1gaa458d98644fdde252a9562c01028ad47}

Checksum algorithm (MDS/SHA1)



#### `public bool clearFailedCache` {#group__pacm_1ga791221a28415eff06ee9a162e950e9dd}

to clear the package cache if installation fails.

This flag tells the package manager weather or not

#### `public inline  Options(const std::string & root)` {#group__pacm_1ga5bf57541a7fc15cae0283c80543060fb}





