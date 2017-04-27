# Module <!-- group --> `sched`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::sched`](#namespacescy_1_1sched)    | 
# namespace `scy::sched` {#namespacescy_1_1sched}



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[`scy::sched::Scheduler`](#classscy_1_1sched_1_1Scheduler)    | 
`class `[`scy::sched::Task`](#classscy_1_1sched_1_1Task)    | 
`class `[`scy::sched::TaskFactory`](#classscy_1_1sched_1_1TaskFactory)    | 
`struct `[`scy::sched::DailyTrigger`](#structscy_1_1sched_1_1DailyTrigger)    | 
`struct `[`scy::sched::IntervalTrigger`](#structscy_1_1sched_1_1IntervalTrigger)    | 
`struct `[`scy::sched::OnceOnlyTrigger`](#structscy_1_1sched_1_1OnceOnlyTrigger)    | 
`struct `[`scy::sched::Trigger`](#structscy_1_1sched_1_1Trigger)    | 
# class `scy::sched::Scheduler` {#classscy_1_1sched_1_1Scheduler}

```
class scy::sched::Scheduler
  : public scy::TaskRunner
  : public scy::json::ISerializable
```  



The [Scheduler](#classscy_1_1sched_1_1Scheduler) manages and runs tasks that need to be executed at specific times.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Scheduler()` | 
`public virtual  ~Scheduler()` | 
`public virtual void schedule(`[`sched::Task`](#classscy_1_1sched_1_1Task)` * task)` | 
`public virtual void cancel(`[`sched::Task`](#classscy_1_1sched_1_1Task)` * task)` | 
`public virtual void clear()` | Destroys and clears all manages tasks.
`public virtual void serialize(json::value & root)` | 
`public virtual void deserialize(json::value & root)` | 
`public virtual void print(std::ostream & ost)` | 
`protected virtual void run()` | Called by the async context to run the next task.
`protected virtual void update()` | 

## Members

#### `public  Scheduler()` {#group__sched_1ga2c5d8bd2230a1f7bf4f073f28110d936}





#### `public virtual  ~Scheduler()` {#group__sched_1ga4a1c46d3daf229f48641fe62d73b3d8f}





#### `public virtual void schedule(`[`sched::Task`](#classscy_1_1sched_1_1Task)` * task)` {#group__sched_1gab33387aa6f9c21d61dbd9ed75da33112}





#### `public virtual void cancel(`[`sched::Task`](#classscy_1_1sched_1_1Task)` * task)` {#group__sched_1ga6dd66192cc1d853e8aea46206f9deeb0}





#### `public virtual void clear()` {#group__sched_1gad1100d94ca6a33a5cd7e12ec4b414ebb}

Destroys and clears all manages tasks.



#### `public virtual void serialize(json::value & root)` {#group__sched_1ga61a4f550053582401cc5d5d59af07b46}





#### `public virtual void deserialize(json::value & root)` {#group__sched_1gaf4241fdeda3188ebbbd02476869e53dd}





#### `public virtual void print(std::ostream & ost)` {#group__sched_1gace1e075e5d80427459eb6a9f431d82c7}





#### `protected virtual void run()` {#group__sched_1ga73b7f819a13ba3938fda58104a96073b}

Called by the async context to run the next task.



#### `protected virtual void update()` {#group__sched_1ga59ae89e12b6fc6cc7d4d3acf9aeb5138}





# class `scy::sched::Task` {#classscy_1_1sched_1_1Task}

```
class scy::sched::Task
  : public scy::Task
  : public scy::json::ISerializable
```  



This class extends the [Task](#classscy_1_1sched_1_1Task) class to implement scheduling capabilities.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  Task(const std::string & type,const std::string & name)` | 
`public  Task(`[`Scheduler`](#classscy_1_1sched_1_1Scheduler)` & scheduler,const std::string & type,const std::string & name)` | 
`public virtual void serialize(json::value & root)` | Serializes the task to JSON.
`public virtual void deserialize(json::value & root)` | Deserializes the task from JSON.
`public template<typename T>`  <br/>`inline T * createTrigger()` | 
`public void setTrigger(`[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` * trigger)` | 
`public `[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` & trigger()` | 
`public `[`Scheduler`](#classscy_1_1sched_1_1Scheduler)` & scheduler()` | 
`public std::int64_t remaining() const` | 
`public std::string type() const` | 
`public std::string name() const` | 
`public void setName(const std::string & name)` | 
`protected std::string _type` | 
`protected std::string _name` | 
`protected `[`sched::Scheduler`](./doc/api-sched.md#classscy_1_1sched_1_1Scheduler)` * _scheduler` | 
`protected `[`sched::Trigger`](./doc/api-sched.md#structscy_1_1sched_1_1Trigger)` * _trigger` | 
`protected mutable std::mutex _mutex` | 
`protected virtual  ~Task()` | Should remain protected.
`protected virtual bool beforeRun()` | 
`protected void run()` | 
`protected virtual bool afterRun()` | 

## Members

#### `public  Task(const std::string & type,const std::string & name)` {#group__sched_1gada7aa3c08477dd31f77a15d9ab7ada69}





#### `public  Task(`[`Scheduler`](#classscy_1_1sched_1_1Scheduler)` & scheduler,const std::string & type,const std::string & name)` {#group__sched_1ga18514602a261599745e9d632a9298639}





#### `public virtual void serialize(json::value & root)` {#group__sched_1ga24bbbb25fbce4353106a3155e2e75526}

Serializes the task to JSON.



#### `public virtual void deserialize(json::value & root)` {#group__sched_1ga42550763109d599e836ff34cdde4017d}

Deserializes the task from JSON.



#### `public template<typename T>`  <br/>`inline T * createTrigger()` {#group__sched_1ga7a6e97531c0903f25d6d7486112cd6ef}





#### `public void setTrigger(`[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` * trigger)` {#group__sched_1ga750447b4c974d32fa66e6bf382968364}





#### `public `[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` & trigger()` {#group__sched_1gaeca4e399907b97a311542cf3f6c02ac5}



Returns a reference to the associated [sched::Trigger](#structscy_1_1sched_1_1Trigger) or throws an exception.

#### `public `[`Scheduler`](#classscy_1_1sched_1_1Scheduler)` & scheduler()` {#group__sched_1ga194bdc6aa8624503a044fe24e1b1ed34}



Returns a reference to the associated [Scheduler](#classscy_1_1sched_1_1Scheduler) or throws an exception.

#### `public std::int64_t remaining() const` {#group__sched_1ga5c18d3c47370a09baaff18b12ecb6779}



Returns the milliseconds remaining until the next scheduled timeout. An [sched::Trigger](#structscy_1_1sched_1_1Trigger) must be associated or an exception will be thrown.

#### `public std::string type() const` {#group__sched_1gab2487a3713a57261b71454c05621dbcd}





#### `public std::string name() const` {#group__sched_1gadfe739e111da7d1d090c88de52da1767}





#### `public void setName(const std::string & name)` {#group__sched_1gaf82f66d275ae0a0b750c3ef3b8785093}





#### `protected std::string _type` {#group__sched_1gac28fe32991a883c1b00a6b56bc555c35}





#### `protected std::string _name` {#group__sched_1ga5c053d1a5d387b942e8cb55a9bfc2fac}





#### `protected `[`sched::Scheduler`](./doc/api-sched.md#classscy_1_1sched_1_1Scheduler)` * _scheduler` {#group__sched_1ga1fe5675a303013606d55d54b79700557}





#### `protected `[`sched::Trigger`](./doc/api-sched.md#structscy_1_1sched_1_1Trigger)` * _trigger` {#group__sched_1ga6627c1994358922d5dc892df16d7ab6e}





#### `protected mutable std::mutex _mutex` {#group__sched_1ga1233679a60cc60f163e2e63988701747}





#### `protected virtual  ~Task()` {#group__sched_1ga92fdc84e904656f4e38cdc7acd8d6053}

Should remain protected.



#### `protected virtual bool beforeRun()` {#group__sched_1gabcf93ea5a1c4c307524ce3711434ee39}





#### `protected void run()` {#group__sched_1ga3d2d7a26a59ef58967926367d6ba484a}



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

#### `protected virtual bool afterRun()` {#group__sched_1gae7f7a8c0ead6727644d3aa045b0c25df}





# class `scy::sched::TaskFactory` {#classscy_1_1sched_1_1TaskFactory}




The [TaskFactory](#classscy_1_1sched_1_1TaskFactory) can dynamically instantiate registered [sched::Task](#classscy_1_1sched_1_1Task) and [sched::Trigger](#structscy_1_1sched_1_1Trigger) classes from named strings.

## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public inline `[`sched::Task`](#classscy_1_1sched_1_1Task)` * createTask(const std::string & type)` | 
`public template<typename T>`  <br/>`inline void registerTask(const std::string & type)` | 
`public inline void unregisterTask(const std::string & type)` | 
`public inline `[`TaskMap`](#group__sched_1ga28cc2ed06a4154af8aa404b2ceee967e)` tasks() const` | 
`public inline `[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` * createTrigger(const std::string & type)` | 
`public template<typename T>`  <br/>`inline void registerTrigger(const std::string & type)` | 
`public inline void unregisterTrigger(const std::string & type)` | 
`public inline TriggerMap triggers() const` | 
`protected mutable std::mutex _mutex` | 
`protected `[`TaskMap`](#group__sched_1ga28cc2ed06a4154af8aa404b2ceee967e)` _tasks` | 
`protected TriggerMap _triggers` | 

## Members

#### `public inline `[`sched::Task`](#classscy_1_1sched_1_1Task)` * createTask(const std::string & type)` {#group__sched_1ga9fe95913f99c0de0f48125fab9804507}





#### `public template<typename T>`  <br/>`inline void registerTask(const std::string & type)` {#group__sched_1ga3b265588e84942014d4d8ed689d71b2d}





#### `public inline void unregisterTask(const std::string & type)` {#group__sched_1ga16d6fa9512bcf4e4b3deb0f1f6807286}





#### `public inline `[`TaskMap`](#group__sched_1ga28cc2ed06a4154af8aa404b2ceee967e)` tasks() const` {#group__sched_1ga41531034b6b18d3a04b04279fe69f587}





#### `public inline `[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` * createTrigger(const std::string & type)` {#group__sched_1ga9c1a5a4b25bfc8522dc02cd5526e18d2}





#### `public template<typename T>`  <br/>`inline void registerTrigger(const std::string & type)` {#group__sched_1ga1cd0703715177bfc010ac24975120e9b}





#### `public inline void unregisterTrigger(const std::string & type)` {#group__sched_1gac05f05edd58089bd2fd37031dd3a2785}





#### `public inline TriggerMap triggers() const` {#group__sched_1ga4c9f42c5dd1ae286d5d4c1e515740e50}





#### `protected mutable std::mutex _mutex` {#group__sched_1gad8e95336c38e4f191d7f27975a8543a4}





#### `protected `[`TaskMap`](#group__sched_1ga28cc2ed06a4154af8aa404b2ceee967e)` _tasks` {#group__sched_1ga852d826a6712845f19aafa5d5588c4fc}





#### `protected TriggerMap _triggers` {#group__sched_1gaa63f76a92ad76b48cea935bda6ac7535}





# struct `scy::sched::DailyTrigger` {#structscy_1_1sched_1_1DailyTrigger}

```
struct scy::sched::DailyTrigger
  : public scy::sched::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` timeOfDay` | 
`public std::vector< `[`DaysOfTheWeek`](#group__sched_1ga215a79a96d62b633ae6a1996fa93dcfe)` > daysExcluded` | 
`public  DailyTrigger()` | 
`public virtual void update()` | 

## Members

#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` timeOfDay` {#group__sched_1gac27c59a1258d294183119fd52ac5b4fc}



This value represents the time of day the task will trigger. The date part of the timestamp is redundant.

#### `public std::vector< `[`DaysOfTheWeek`](#group__sched_1ga215a79a96d62b633ae6a1996fa93dcfe)` > daysExcluded` {#group__sched_1ga336f20188705b1fa837686bb01898409}



Days of the week can be excluded by adding the appropriate bit flag here.

#### `public  DailyTrigger()` {#group__sched_1ga5bbeff00daa5b34e548ec34276c599b0}





#### `public virtual void update()` {#group__sched_1gaaeb2ec2d9aa66f5e4b776fa7adce4405}



Updates the scheduleAt value to the next scheduled time.

# struct `scy::sched::IntervalTrigger` {#structscy_1_1sched_1_1IntervalTrigger}

```
struct scy::sched::IntervalTrigger
  : public scy::sched::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Timespan`](./doc/api-base.md#classscy_1_1Timespan)` interval` | 
`public int maxTimes` | 
`public  IntervalTrigger()` | 
`public virtual void update()` | 
`public virtual bool expired()` | 
`public virtual void serialize(json::value & root)` | 
`public virtual void deserialize(json::value & root)` | 

## Members

#### `public `[`Timespan`](./doc/api-base.md#classscy_1_1Timespan)` interval` {#group__sched_1ga79c8a845365cd31f534b730e022e8d4d}



This value represents the interval to wait before running the task.

#### `public int maxTimes` {#group__sched_1ga80f340cb8d0f23d4b1a89b2e200518b8}



The maximum number of times the task will be run before it is destroyed. 0 for no effect.

#### `public  IntervalTrigger()` {#group__sched_1gad32ce801f384e0a4a4a4e19acfe3a34c}





#### `public virtual void update()` {#group__sched_1ga63a1a00bf076fe3971d369960bae3805}



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` {#group__sched_1gad97ce8175cfbcfb9058ed879a9a64777}



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::value & root)` {#group__sched_1ga84598c130a3b1956d746224c0fd733a3}





#### `public virtual void deserialize(json::value & root)` {#group__sched_1ga7b741fa4beddfa89ec8a0a90b597b701}





# struct `scy::sched::OnceOnlyTrigger` {#structscy_1_1sched_1_1OnceOnlyTrigger}

```
struct scy::sched::OnceOnlyTrigger
  : public scy::sched::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public  OnceOnlyTrigger()` | 
`public inline virtual void update()` | 
`public virtual bool expired()` | 

## Members

#### `public  OnceOnlyTrigger()` {#group__sched_1ga134b95cccf5189e5e707b55437835436}





#### `public inline virtual void update()` {#group__sched_1ga903fa0937aae684c3979f50b0b98e273}



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` {#group__sched_1gacd76b6807128e8fca3579987a6f7c712}



Returns true if the task is expired and should be destroyed. Returns false by default.

# struct `scy::sched::Trigger` {#structscy_1_1sched_1_1Trigger}

```
struct scy::sched::Trigger
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
`public virtual void serialize(json::value & root)` | 
`public virtual void deserialize(json::value & root)` | 

## Members

#### `public std::string type` {#group__sched_1ga80dbc210eee2539299264057d881959a}

The type of this trigger class.



#### `public std::string name` {#group__sched_1ga3e24e172d92dc417863569d8b95323d9}

The display name of this trigger class.



#### `public int timesRun` {#group__sched_1ga16517ce07a40cd2e66fd8065fa998b7b}



The number of times the task has run since creation;

#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` createdAt` {#group__sched_1ga36015f8af492436ca74de13d7a43ce79}

The time the task was created.



#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` scheduleAt` {#group__sched_1ga6435f36f242fb144219d7b05600a4e1c}

The time the task is scheduled to run.



#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` lastRunAt` {#group__sched_1ga9ed36dda7c8943d3644e295774a9a213}

The time the task was last run.



#### `public  Trigger(const std::string & type,const std::string & name)` {#group__sched_1ga1d108947ea17eb1787d2bbd9305dda59}





#### `public void update()` {#group__sched_1ga5deb765be5af1396d7cdfb2904c426d8}



Updates the scheduleAt value to the next scheduled time.

#### `public virtual std::int64_t remaining()` {#group__sched_1gaaab35f447a018bbda0742b5bd9992f71}



Returns the milliseconds remaining until the next scheduled timeout.

#### `public virtual bool timeout()` {#group__sched_1gaa20be8a2498f10c86bdb7bf87b39cb70}



Returns true if the task is ready to be run, false otherwise.

#### `public virtual bool expired()` {#group__sched_1ga6085d6dc573d7034cbf55f70749f6e73}



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::value & root)` {#group__sched_1gaf0765c831e5f8be8641cd0e19bbf2667}





#### `public virtual void deserialize(json::value & root)` {#group__sched_1ga218b2abde39d1d80d6cd7dea79486c84}





