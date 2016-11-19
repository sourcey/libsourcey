# Module <!-- group --> `sked`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::sked`](#namespacescy_1_1sked)    | 
# namespace `scy::sked` {#namespacescy_1_1sked}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::sked::Scheduler`](#classscy_1_1sked_1_1Scheduler)    | 
`class `[`scy::sked::Task`](#classscy_1_1sked_1_1Task)    | 
`class `[`scy::sked::TaskFactory`](#classscy_1_1sked_1_1TaskFactory)    | 
`struct `[`scy::sked::DailyTrigger`](#structscy_1_1sked_1_1DailyTrigger)    | 
`struct `[`scy::sked::IntervalTrigger`](#structscy_1_1sked_1_1IntervalTrigger)    | 
`struct `[`scy::sked::OnceOnlyTrigger`](#structscy_1_1sked_1_1OnceOnlyTrigger)    | 
`struct `[`scy::sked::Trigger`](#structscy_1_1sked_1_1Trigger)    | 
# class `scy::sked::Scheduler` {#classscy_1_1sked_1_1Scheduler}

```
class scy::sked::Scheduler
  : public scy::TaskRunner
  : public scy::json::ISerializable
```  



The [Scheduler](#classscy_1_1sked_1_1Scheduler) manages and runs tasks that need to be executed at specific times.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Scheduler()` | 
`public virtual  ~Scheduler()` | 
`public virtual void schedule(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` | 
`public virtual void cancel(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` | 
`public virtual void clear()` | Destroys and clears all manages tasks.
`public virtual void serialize(json::Value & root)` | 
`public virtual void deserialize(json::Value & root)` | 
`public virtual void print(std::ostream & ost)` | 
`protected virtual void run()` | Called by the async context to run the next task.
`protected virtual void update()` | 

## Members

#### `public  Scheduler()` {#group__sked_1ga514a0aa9634b1951ae56a99564caeb33}





#### `public virtual  ~Scheduler()` {#group__sked_1ga48b23def7201e4a5037a35a673cb3f97}





#### `public virtual void schedule(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` {#group__sked_1gacc21c6934f5af42c17f00f8eaeb779c8}





#### `public virtual void cancel(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` {#group__sked_1ga0c63154c5dd7f3f7d78409102fa761f4}





#### `public virtual void clear()` {#group__sked_1ga8df4c68754fab027eb3e30f44311df1e}

Destroys and clears all manages tasks.



#### `public virtual void serialize(json::Value & root)` {#group__sked_1gadd053ba59d1f38a08e041f3a42b8f9e2}





#### `public virtual void deserialize(json::Value & root)` {#group__sked_1gaa6b3a781c8e69f6de99ad05b7b1d1d63}





#### `public virtual void print(std::ostream & ost)` {#group__sked_1gaaab06d2eb415e90490cd565d749c5638}





#### `protected virtual void run()` {#group__sked_1ga74363bb7cb688746b40d479d950f6975}

Called by the async context to run the next task.



#### `protected virtual void update()` {#group__sked_1ga67269cfef7d229e4a24f5bfc92bc339d}





# class `scy::sked::Task` {#classscy_1_1sked_1_1Task}

```
class scy::sked::Task
  : public scy::Task
  : public scy::json::ISerializable
```  



This class extends the [Task](#classscy_1_1sked_1_1Task) class to implement scheduling capabilities.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Task(const std::string & type,const std::string & name)` | 
`public  Task(`[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler,const std::string & type,const std::string & name)` | 
`public virtual void serialize(json::Value & root)` | Serializes the task to JSON.
`public virtual void deserialize(json::Value & root)` | Deserializes the task from JSON.
`public template<typename T>`  <br/>`inline T * createTrigger()` | 
`public void setTrigger(`[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * trigger)` | 
`public `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` & trigger()` | 
`public `[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler()` | 
`public std::int64_t remaining() const` | 
`public std::string type() const` | 
`public std::string name() const` | 
`public void setName(const std::string & name)` | 
`protected std::string _type` | 
`protected std::string _name` | 
`protected `[`sked::Scheduler`](./doc/api-sked.md#classscy_1_1sked_1_1Scheduler)` * _scheduler` | 
`protected `[`sked::Trigger`](./doc/api-sked.md#structscy_1_1sked_1_1Trigger)` * _trigger` | 
`protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` | 
`protected virtual  ~Task()` | Should remain protected.
`protected virtual bool beforeRun()` | 
`protected void run()` | 
`protected virtual bool afterRun()` | 

## Members

#### `public  Task(const std::string & type,const std::string & name)` {#group__sked_1ga386a15d521531a67fdb090824da93890}





#### `public  Task(`[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler,const std::string & type,const std::string & name)` {#group__sked_1gab06c7ef2f60c5486229cd426919cc36c}





#### `public virtual void serialize(json::Value & root)` {#group__sked_1ga2d10fe6de9492ad784039a4e257eced5}

Serializes the task to JSON.



#### `public virtual void deserialize(json::Value & root)` {#group__sked_1gac0c3e0cfa2b24a12d2f72488f62b4d09}

Deserializes the task from JSON.



#### `public template<typename T>`  <br/>`inline T * createTrigger()` {#group__sked_1ga1a8b7a4f4fc7e1dcc06b5c4454c875ab}





#### `public void setTrigger(`[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * trigger)` {#group__sked_1ga3d72bcdb505450e88522b17dddd9b73b}





#### `public `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` & trigger()` {#group__sked_1ga79f463a50c3a8ee9cd482f7bce8d67ea}



Returns a reference to the associated [sked::Trigger](#structscy_1_1sked_1_1Trigger) or throws an exception.

#### `public `[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler()` {#group__sked_1ga8d439f1522684b89f81b3852348019bd}



Returns a reference to the associated [Scheduler](#classscy_1_1sked_1_1Scheduler) or throws an exception.

#### `public std::int64_t remaining() const` {#group__sked_1ga35f659f3ebe2737385595564402d0f54}



Returns the milliseconds remaining until the next scheduled timeout. An [sked::Trigger](#structscy_1_1sked_1_1Trigger) must be associated or an exception will be thrown.

#### `public std::string type() const` {#group__sked_1ga83e6c392d631a66fd5f803a806e9e502}





#### `public std::string name() const` {#group__sked_1gac054517b6cd13fa107c629465e36c8bd}





#### `public void setName(const std::string & name)` {#group__sked_1ga0d1f0b5a756774917f742f38acc7cefe}





#### `protected std::string _type` {#group__sked_1ga593c8a61cdc6f5775aa26c83699f7acc}





#### `protected std::string _name` {#group__sked_1ga0cff879b589e588e38589eef0f8329b2}





#### `protected `[`sked::Scheduler`](./doc/api-sked.md#classscy_1_1sked_1_1Scheduler)` * _scheduler` {#group__sked_1ga7a67dcc93b0781f5e8579bb888951c6d}





#### `protected `[`sked::Trigger`](./doc/api-sked.md#structscy_1_1sked_1_1Trigger)` * _trigger` {#group__sked_1ga29d12af7fc6b09ae0ea2120a21179f4a}





#### `protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` {#group__sked_1gacc6e9cb2c3332ce7e3856ad20053fd4f}





#### `protected virtual  ~Task()` {#group__sked_1ga27c4083f96354bccaa7b2b24b5404e7b}

Should remain protected.



#### `protected virtual bool beforeRun()` {#group__sked_1gaa4e46a5d56fe059de12ebec07538b343}





#### `protected void run()` {#group__sked_1gaa9b1a4ffaa4e3e9d1ec795b1af35c0b9}



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

#### `protected virtual bool afterRun()` {#group__sked_1gaf1e8643d12b32391169a1fa66cd9532b}





# class `scy::sked::TaskFactory` {#classscy_1_1sked_1_1TaskFactory}




The [TaskFactory](#classscy_1_1sked_1_1TaskFactory) can dynamically instantiate registered [sked::Task](#classscy_1_1sked_1_1Task) and [sked::Trigger](#structscy_1_1sked_1_1Trigger) classes from named strings.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline `[`sked::Task`](#classscy_1_1sked_1_1Task)` * createTask(const std::string & type)` | 
`public template<typename T>`  <br/>`inline void registerTask(const std::string & type)` | 
`public inline void unregisterTask(const std::string & type)` | 
`public inline TaskMap tasks() const` | 
`public inline `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * createTrigger(const std::string & type)` | 
`public template<typename T>`  <br/>`inline void registerTrigger(const std::string & type)` | 
`public inline void unregisterTrigger(const std::string & type)` | 
`public inline TriggerMap triggers() const` | 
`protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` | 
`protected TaskMap _tasks` | 
`protected TriggerMap _triggers` | 

## Members

#### `public inline `[`sked::Task`](#classscy_1_1sked_1_1Task)` * createTask(const std::string & type)` {#group__sked_1gaf2c3154913174136d643907915637772}





#### `public template<typename T>`  <br/>`inline void registerTask(const std::string & type)` {#group__sked_1ga6abe68ae8192c8a724bfb8297dc2fb83}





#### `public inline void unregisterTask(const std::string & type)` {#group__sked_1ga9279b37d8ed19efce7ed4a44fe14bcdb}





#### `public inline TaskMap tasks() const` {#group__sked_1ga1810c49aadc9105cc18f4fd4eee54e8d}





#### `public inline `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * createTrigger(const std::string & type)` {#group__sked_1ga6b74f8b76c41cfa76b2b966dc4f04728}





#### `public template<typename T>`  <br/>`inline void registerTrigger(const std::string & type)` {#group__sked_1gad7739778b7ea92b9164fabfd61f3162c}





#### `public inline void unregisterTrigger(const std::string & type)` {#group__sked_1gaa82d4cb06fab8254b68fe95040dd0772}





#### `public inline TriggerMap triggers() const` {#group__sked_1ga54a36cb63df452507f257c9bd02a31ea}





#### `protected mutable `[`Mutex`](./doc/api-base.md#classscy_1_1Mutex)` _mutex` {#group__sked_1gaba4115abe13876bea87f674ce7354295}





#### `protected TaskMap _tasks` {#group__sked_1ga83f4f9401b6f52622ca74c4d884d4e77}





#### `protected TriggerMap _triggers` {#group__sked_1ga7f0a4e67c56a5f0a997cd2caa993b187}





# struct `scy::sked::DailyTrigger` {#structscy_1_1sked_1_1DailyTrigger}

```
struct scy::sked::DailyTrigger
  : public scy::sked::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` timeOfDay` | 
`public std::vector< `[`DaysOfTheWeek`](#group__sked_1gafaf09924bf11ff91312d278879efc364)` > daysExcluded` | 
`public  DailyTrigger()` | 
`public virtual void update()` | 

## Members

#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` timeOfDay` {#group__sked_1ga0f50994338086f658223bb591aaa855b}



This value represents the time of day the task will trigger. The date part of the timestamp is redundant.

#### `public std::vector< `[`DaysOfTheWeek`](#group__sked_1gafaf09924bf11ff91312d278879efc364)` > daysExcluded` {#group__sked_1gaeecee22b3f9a186f36b37c3967c84261}



Days of the week can be excluded by adding the appropriate bit flag here.

#### `public  DailyTrigger()` {#group__sked_1gaefddcdf43f7dd40d2fd9c4973aa16f5c}





#### `public virtual void update()` {#group__sked_1ga56d431a340ef90a63c3a9ef7c5fecdac}



Updates the scheduleAt value to the next scheduled time.

# struct `scy::sked::IntervalTrigger` {#structscy_1_1sked_1_1IntervalTrigger}

```
struct scy::sked::IntervalTrigger
  : public scy::sked::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Timespan`](./doc/api-base.md#classscy_1_1Timespan)` interval` | 
`public int maxTimes` | 
`public  IntervalTrigger()` | 
`public virtual void update()` | 
`public virtual bool expired()` | 
`public virtual void serialize(json::Value & root)` | 
`public virtual void deserialize(json::Value & root)` | 

## Members

#### `public `[`Timespan`](./doc/api-base.md#classscy_1_1Timespan)` interval` {#group__sked_1ga8f86f84623d9cdd251cb596d1b9cd7ea}



This value represents the interval to wait before running the task.

#### `public int maxTimes` {#group__sked_1ga8e40cfaeaa7e36c6e8fecb557fa6fe4a}



The maximum number of times the task will be run before it is destroyed. 0 for no effect.

#### `public  IntervalTrigger()` {#group__sked_1ga2a6fcdbba2933d6f1a0c2e60e75a1afa}





#### `public virtual void update()` {#group__sked_1ga01c689ed43d5fb9981303ca9a57a2623}



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` {#group__sked_1gae9ebbb2da9675cf0f861105b406ec827}



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::Value & root)` {#group__sked_1gabae55c2745920c5b40daa2ff5fc23b66}





#### `public virtual void deserialize(json::Value & root)` {#group__sked_1ga345231c106ed12e9a2ff6c30c91b3280}





# struct `scy::sked::OnceOnlyTrigger` {#structscy_1_1sked_1_1OnceOnlyTrigger}

```
struct scy::sked::OnceOnlyTrigger
  : public scy::sked::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  OnceOnlyTrigger()` | 
`public inline virtual void update()` | 
`public virtual bool expired()` | 

## Members

#### `public  OnceOnlyTrigger()` {#group__sked_1ga415a7be1c65cda0e938c2c71947650e5}





#### `public inline virtual void update()` {#group__sked_1ga357a04a19f44aec0fff439398b574634}



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` {#group__sked_1ga578595dc276d57bce1a54e7ba3fd3da1}



Returns true if the task is expired and should be destroyed. Returns false by default.

# struct `scy::sked::Trigger` {#structscy_1_1sked_1_1Trigger}

```
struct scy::sked::Trigger
  : public scy::json::ISerializable
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public std::string type` | The type of this trigger class.
`public std::string name` | The display name of this trigger class.
`public int timesRun` | 
`public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` createdAt` | The time the task was created.
`public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` scheduleAt` | The time the task is scheduled to run.
`public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` lastRunAt` | The time the task was last run.
`public  Trigger(const std::string & type,const std::string & name)` | 
`public void update()` | 
`public virtual std::int64_t remaining()` | 
`public virtual bool timeout()` | 
`public virtual bool expired()` | 
`public virtual void serialize(json::Value & root)` | 
`public virtual void deserialize(json::Value & root)` | 

## Members

#### `public std::string type` {#group__sked_1ga7cc1c5928a3bf2de63df3e6e7d4cd413}

The type of this trigger class.



#### `public std::string name` {#group__sked_1gaa49fe31e4a8bc141f9ded29e305ca322}

The display name of this trigger class.



#### `public int timesRun` {#group__sked_1ga9d336b4c320364d7a86762039418531c}



The number of times the task has run since creation;

#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` createdAt` {#group__sked_1gaf0f5c1e386f877fd8f2d099d441093f3}

The time the task was created.



#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` scheduleAt` {#group__sked_1ga1063f958b835c6d033c8ff389b2e94f0}

The time the task is scheduled to run.



#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` lastRunAt` {#group__sked_1ga3fdacd4cae9a26840ff138aa48bbe4e1}

The time the task was last run.



#### `public  Trigger(const std::string & type,const std::string & name)` {#group__sked_1ga6c5824c78e581a00b0d440466584bd22}





#### `public void update()` {#group__sked_1gaf1f2ee5a783e1c9190430b07fc89f8da}



Updates the scheduleAt value to the next scheduled time.

#### `public virtual std::int64_t remaining()` {#group__sked_1ga01473ad840be6c629fc360a3d4750eca}



Returns the milliseconds remaining until the next scheduled timeout.

#### `public virtual bool timeout()` {#group__sked_1gadb3c28f4bb3af12a90422eff0c42dece}



Returns true if the task is ready to be run, false otherwise.

#### `public virtual bool expired()` {#group__sked_1ga2c6d97224c1bd3aaa8cb7fd70b6bc1c1}



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::Value & root)` {#group__sked_1ga263a8007f39041e235c835b967a61f93}





#### `public virtual void deserialize(json::Value & root)` {#group__sked_1ga480d0f52b3319d765e5c6b809c946609}





