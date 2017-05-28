# Module <!-- group --> `sched`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[`scy::sched`](#namespacescy_1_1sched)    | 
# namespace `scy::sched` 



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
# class `scy::sched::Scheduler` 

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

#### `public  Scheduler()` 





#### `public virtual  ~Scheduler()` 





#### `public virtual void schedule(`[`sched::Task`](#classscy_1_1sched_1_1Task)` * task)` 





#### `public virtual void cancel(`[`sched::Task`](#classscy_1_1sched_1_1Task)` * task)` 





#### `public virtual void clear()` 

Destroys and clears all manages tasks.



#### `public virtual void serialize(json::value & root)` 





#### `public virtual void deserialize(json::value & root)` 





#### `public virtual void print(std::ostream & ost)` 





#### `protected virtual void run()` 

Called by the async context to run the next task.



#### `protected virtual void update()` 





# class `scy::sched::Task` 

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
`protected virtual  ~Task()` | 
`protected virtual bool beforeRun()` | 
`protected void run()` | 
`protected virtual bool afterRun()` | 

## Members

#### `public  Task(const std::string & type,const std::string & name)` 





#### `public  Task(`[`Scheduler`](#classscy_1_1sched_1_1Scheduler)` & scheduler,const std::string & type,const std::string & name)` 





#### `public virtual void serialize(json::value & root)` 

Serializes the task to JSON.



#### `public virtual void deserialize(json::value & root)` 

Deserializes the task from JSON.



#### `public template<typename T>`  <br/>`inline T * createTrigger()` 





#### `public void setTrigger(`[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` * trigger)` 





#### `public `[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` & trigger()` 



Returns a reference to the associated [sched::Trigger](#structscy_1_1sched_1_1Trigger) or throws an exception.

#### `public `[`Scheduler`](#classscy_1_1sched_1_1Scheduler)` & scheduler()` 



Returns a reference to the associated [Scheduler](#classscy_1_1sched_1_1Scheduler) or throws an exception.

#### `public std::int64_t remaining() const` 



Returns the milliseconds remaining until the next scheduled timeout. An [sched::Trigger](#structscy_1_1sched_1_1Trigger) must be associated or an exception will be thrown.

#### `public std::string type() const` 





#### `public std::string name() const` 





#### `public void setName(const std::string & name)` 





#### `protected std::string _type` 





#### `protected std::string _name` 





#### `protected `[`sched::Scheduler`](./doc/api-sched.md#classscy_1_1sched_1_1Scheduler)` * _scheduler` 





#### `protected `[`sched::Trigger`](./doc/api-sched.md#structscy_1_1sched_1_1Trigger)` * _trigger` 





#### `protected mutable std::mutex _mutex` 





#### `protected virtual  ~Task()` 



Destroctor. Should remain protected.

#### `protected virtual bool beforeRun()` 





#### `protected void run()` 



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

#### `protected virtual bool afterRun()` 





# class `scy::sched::TaskFactory` 




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

#### `public inline `[`sched::Task`](#classscy_1_1sched_1_1Task)` * createTask(const std::string & type)` 





#### `public template<typename T>`  <br/>`inline void registerTask(const std::string & type)` 





#### `public inline void unregisterTask(const std::string & type)` 





#### `public inline `[`TaskMap`](#group__sched_1ga28cc2ed06a4154af8aa404b2ceee967e)` tasks() const` 





#### `public inline `[`sched::Trigger`](#structscy_1_1sched_1_1Trigger)` * createTrigger(const std::string & type)` 





#### `public template<typename T>`  <br/>`inline void registerTrigger(const std::string & type)` 





#### `public inline void unregisterTrigger(const std::string & type)` 





#### `public inline TriggerMap triggers() const` 





#### `protected mutable std::mutex _mutex` 





#### `protected `[`TaskMap`](#group__sched_1ga28cc2ed06a4154af8aa404b2ceee967e)` _tasks` 





#### `protected TriggerMap _triggers` 





# struct `scy::sched::DailyTrigger` 

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

#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` timeOfDay` 



This value represents the time of day the task will trigger. The date part of the timestamp is redundant.

#### `public std::vector< `[`DaysOfTheWeek`](#group__sched_1ga215a79a96d62b633ae6a1996fa93dcfe)` > daysExcluded` 



Days of the week can be excluded by adding the appropriate bit flag here.

#### `public  DailyTrigger()` 





#### `public virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

# struct `scy::sched::IntervalTrigger` 

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

#### `public `[`Timespan`](./doc/api-base.md#classscy_1_1Timespan)` interval` 



This value represents the interval to wait before running the task.

#### `public int maxTimes` 



The maximum number of times the task will be run before it is destroyed. 0 for no effect.

#### `public  IntervalTrigger()` 





#### `public virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` 



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::value & root)` 





#### `public virtual void deserialize(json::value & root)` 





# struct `scy::sched::OnceOnlyTrigger` 

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

#### `public  OnceOnlyTrigger()` 





#### `public inline virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` 



Returns true if the task is expired and should be destroyed. Returns false by default.

# struct `scy::sched::Trigger` 

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

#### `public std::string type` 

The type of this trigger class.



#### `public std::string name` 

The display name of this trigger class.



#### `public int timesRun` 



The number of times the task has run since creation;

#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` createdAt` 

The time the task was created.



#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` scheduleAt` 

The time the task is scheduled to run.



#### `public `[`DateTime`](./doc/api-base.md#classscy_1_1DateTime)` lastRunAt` 

The time the task was last run.



#### `public  Trigger(const std::string & type,const std::string & name)` 





#### `public void update()` 



Updates the scheduleAt value to the next scheduled time.

#### `public virtual std::int64_t remaining()` 



Returns the milliseconds remaining until the next scheduled timeout.

#### `public virtual bool timeout()` 



Returns true if the task is ready to be run, false otherwise.

#### `public virtual bool expired()` 



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::value & root)` 





#### `public virtual void deserialize(json::value & root)` 





