# Module <!-- group --> `sked`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`namespace `[``scy::sked``](#namespacescy_1_1sked)    | 
`class `[``scy::sked::Scheduler``](#classscy_1_1sked_1_1Scheduler)    | 
`class `[``scy::sked::Task``](#classscy_1_1sked_1_1Task)    | 
`class `[``scy::sked::TaskFactory``](#classscy_1_1sked_1_1TaskFactory)    | 
`struct `[``scy::sked::Trigger``](#structscy_1_1sked_1_1Trigger)    | 
`struct `[``scy::sked::OnceOnlyTrigger``](#structscy_1_1sked_1_1OnceOnlyTrigger)    | 
`struct `[``scy::sked::IntervalTrigger``](#structscy_1_1sked_1_1IntervalTrigger)    | 
`struct `[``scy::sked::DailyTrigger``](#structscy_1_1sked_1_1DailyTrigger)    | 
# namespace `scy::sked`



## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`class `[``scy::sked::Scheduler``](#classscy_1_1sked_1_1Scheduler)    | 
`class `[``scy::sked::Task``](#classscy_1_1sked_1_1Task)    | 
`class `[``scy::sked::TaskFactory``](#classscy_1_1sked_1_1TaskFactory)    | 
`struct `[``scy::sked::DailyTrigger``](#structscy_1_1sked_1_1DailyTrigger)    | 
`struct `[``scy::sked::IntervalTrigger``](#structscy_1_1sked_1_1IntervalTrigger)    | 
`struct `[``scy::sked::OnceOnlyTrigger``](#structscy_1_1sked_1_1OnceOnlyTrigger)    | 
`struct `[``scy::sked::Trigger``](#structscy_1_1sked_1_1Trigger)    | 
# class `scy::sked::Scheduler` 

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

#### `public  Scheduler()` 





#### `public virtual  ~Scheduler()` 





#### `public virtual void schedule(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` 





#### `public virtual void cancel(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` 





#### `public virtual void clear()` 

Destroys and clears all manages tasks.



#### `public virtual void serialize(json::Value & root)` 





#### `public virtual void deserialize(json::Value & root)` 





#### `public virtual void print(std::ostream & ost)` 





#### `protected virtual void run()` 

Called by the async context to run the next task.



#### `protected virtual void update()` 





# class `scy::sked::Task` 

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
`protected `[`sked::Scheduler`](#classscy_1_1sked_1_1Scheduler)` * _scheduler` | 
`protected `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * _trigger` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected virtual  ~Task()` | Should remain protected.
`protected virtual bool beforeRun()` | 
`protected void run()` | 
`protected virtual bool afterRun()` | 

## Members

#### `public  Task(const std::string & type,const std::string & name)` 





#### `public  Task(`[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler,const std::string & type,const std::string & name)` 





#### `public virtual void serialize(json::Value & root)` 

Serializes the task to JSON.



#### `public virtual void deserialize(json::Value & root)` 

Deserializes the task from JSON.



#### `public template<typename T>`  <br/>`inline T * createTrigger()` 





#### `public void setTrigger(`[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * trigger)` 





#### `public `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` & trigger()` 



Returns a reference to the associated [sked::Trigger](#structscy_1_1sked_1_1Trigger) or throws an exception.

#### `public `[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler()` 



Returns a reference to the associated [Scheduler](#classscy_1_1sked_1_1Scheduler) or throws an exception.

#### `public std::int64_t remaining() const` 



Returns the milliseconds remaining until the next scheduled timeout. An [sked::Trigger](#structscy_1_1sked_1_1Trigger) must be associated or an exception will be thrown.

#### `public std::string type() const` 





#### `public std::string name() const` 





#### `public void setName(const std::string & name)` 





#### `protected std::string _type` 





#### `protected std::string _name` 





#### `protected `[`sked::Scheduler`](#classscy_1_1sked_1_1Scheduler)` * _scheduler` 





#### `protected `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * _trigger` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected virtual  ~Task()` 

Should remain protected.



#### `protected virtual bool beforeRun()` 





#### `protected void run()` 



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

#### `protected virtual bool afterRun()` 





# class `scy::sked::TaskFactory` 




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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected TaskMap _tasks` | 
`protected TriggerMap _triggers` | 

## Members

#### `public inline `[`sked::Task`](#classscy_1_1sked_1_1Task)` * createTask(const std::string & type)` 





#### `public template<typename T>`  <br/>`inline void registerTask(const std::string & type)` 





#### `public inline void unregisterTask(const std::string & type)` 





#### `public inline TaskMap tasks() const` 





#### `public inline `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * createTrigger(const std::string & type)` 





#### `public template<typename T>`  <br/>`inline void registerTrigger(const std::string & type)` 





#### `public inline void unregisterTrigger(const std::string & type)` 





#### `public inline TriggerMap triggers() const` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected TaskMap _tasks` 





#### `protected TriggerMap _triggers` 





# struct `scy::sked::DailyTrigger` 

```
struct scy::sked::DailyTrigger
  : public scy::sked::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`DateTime`](#classscy_1_1DateTime)` timeOfDay` | 
`public std::vector< `[`DaysOfTheWeek`](#group__sked_1gafaf09924bf11ff91312d278879efc364)` > daysExcluded` | 
`public  DailyTrigger()` | 
`public virtual void update()` | 

## Members

#### `public `[`DateTime`](#classscy_1_1DateTime)` timeOfDay` 



This value represents the time of day the task will trigger. The date part of the timestamp is redundant.

#### `public std::vector< `[`DaysOfTheWeek`](#group__sked_1gafaf09924bf11ff91312d278879efc364)` > daysExcluded` 



Days of the week can be excluded by adding the appropriate bit flag here.

#### `public  DailyTrigger()` 





#### `public virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

# struct `scy::sked::IntervalTrigger` 

```
struct scy::sked::IntervalTrigger
  : public scy::sked::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Timespan`](#classscy_1_1Timespan)` interval` | 
`public int maxTimes` | 
`public  IntervalTrigger()` | 
`public virtual void update()` | 
`public virtual bool expired()` | 
`public virtual void serialize(json::Value & root)` | 
`public virtual void deserialize(json::Value & root)` | 

## Members

#### `public `[`Timespan`](#classscy_1_1Timespan)` interval` 



This value represents the interval to wait before running the task.

#### `public int maxTimes` 



The maximum number of times the task will be run before it is destroyed. 0 for no effect.

#### `public  IntervalTrigger()` 





#### `public virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` 



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::Value & root)` 





#### `public virtual void deserialize(json::Value & root)` 





# struct `scy::sked::OnceOnlyTrigger` 

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

#### `public  OnceOnlyTrigger()` 





#### `public inline virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` 



Returns true if the task is expired and should be destroyed. Returns false by default.

# struct `scy::sked::Trigger` 

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
`public `[`DateTime`](#classscy_1_1DateTime)` createdAt` | The time the task was created.
`public `[`DateTime`](#classscy_1_1DateTime)` scheduleAt` | The time the task is scheduled to run.
`public `[`DateTime`](#classscy_1_1DateTime)` lastRunAt` | The time the task was last run.
`public  Trigger(const std::string & type,const std::string & name)` | 
`public void update()` | 
`public virtual std::int64_t remaining()` | 
`public virtual bool timeout()` | 
`public virtual bool expired()` | 
`public virtual void serialize(json::Value & root)` | 
`public virtual void deserialize(json::Value & root)` | 

## Members

#### `public std::string type` 

The type of this trigger class.



#### `public std::string name` 

The display name of this trigger class.



#### `public int timesRun` 



The number of times the task has run since creation;

#### `public `[`DateTime`](#classscy_1_1DateTime)` createdAt` 

The time the task was created.



#### `public `[`DateTime`](#classscy_1_1DateTime)` scheduleAt` 

The time the task is scheduled to run.



#### `public `[`DateTime`](#classscy_1_1DateTime)` lastRunAt` 

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

#### `public virtual void serialize(json::Value & root)` 





#### `public virtual void deserialize(json::Value & root)` 





# class `scy::sked::Scheduler` 

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

#### `public  Scheduler()` 





#### `public virtual  ~Scheduler()` 





#### `public virtual void schedule(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` 





#### `public virtual void cancel(`[`sked::Task`](#classscy_1_1sked_1_1Task)` * task)` 





#### `public virtual void clear()` 

Destroys and clears all manages tasks.



#### `public virtual void serialize(json::Value & root)` 





#### `public virtual void deserialize(json::Value & root)` 





#### `public virtual void print(std::ostream & ost)` 





#### `protected virtual void run()` 

Called by the async context to run the next task.



#### `protected virtual void update()` 





# class `scy::sked::Task` 

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
`protected `[`sked::Scheduler`](#classscy_1_1sked_1_1Scheduler)` * _scheduler` | 
`protected `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * _trigger` | 
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected virtual  ~Task()` | Should remain protected.
`protected virtual bool beforeRun()` | 
`protected void run()` | 
`protected virtual bool afterRun()` | 

## Members

#### `public  Task(const std::string & type,const std::string & name)` 





#### `public  Task(`[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler,const std::string & type,const std::string & name)` 





#### `public virtual void serialize(json::Value & root)` 

Serializes the task to JSON.



#### `public virtual void deserialize(json::Value & root)` 

Deserializes the task from JSON.



#### `public template<typename T>`  <br/>`inline T * createTrigger()` 





#### `public void setTrigger(`[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * trigger)` 





#### `public `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` & trigger()` 



Returns a reference to the associated [sked::Trigger](#structscy_1_1sked_1_1Trigger) or throws an exception.

#### `public `[`Scheduler`](#classscy_1_1sked_1_1Scheduler)` & scheduler()` 



Returns a reference to the associated [Scheduler](#classscy_1_1sked_1_1Scheduler) or throws an exception.

#### `public std::int64_t remaining() const` 



Returns the milliseconds remaining until the next scheduled timeout. An [sked::Trigger](#structscy_1_1sked_1_1Trigger) must be associated or an exception will be thrown.

#### `public std::string type() const` 





#### `public std::string name() const` 





#### `public void setName(const std::string & name)` 





#### `protected std::string _type` 





#### `protected std::string _name` 





#### `protected `[`sked::Scheduler`](#classscy_1_1sked_1_1Scheduler)` * _scheduler` 





#### `protected `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * _trigger` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected virtual  ~Task()` 

Should remain protected.



#### `protected virtual bool beforeRun()` 





#### `protected void run()` 



Called by the [TaskRunner](#classscy_1_1TaskRunner) to run the task. Override this method to implement task action. Returning true means the true should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed id [destroy()](#group__base_1gaecd8a85018f8f75ae802dcbf3749bdef) was called during the current task iteration.

#### `protected virtual bool afterRun()` 





# class `scy::sked::TaskFactory` 




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
`protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` | 
`protected TaskMap _tasks` | 
`protected TriggerMap _triggers` | 

## Members

#### `public inline `[`sked::Task`](#classscy_1_1sked_1_1Task)` * createTask(const std::string & type)` 





#### `public template<typename T>`  <br/>`inline void registerTask(const std::string & type)` 





#### `public inline void unregisterTask(const std::string & type)` 





#### `public inline TaskMap tasks() const` 





#### `public inline `[`sked::Trigger`](#structscy_1_1sked_1_1Trigger)` * createTrigger(const std::string & type)` 





#### `public template<typename T>`  <br/>`inline void registerTrigger(const std::string & type)` 





#### `public inline void unregisterTrigger(const std::string & type)` 





#### `public inline TriggerMap triggers() const` 





#### `protected mutable `[`Mutex`](#classscy_1_1Mutex)` _mutex` 





#### `protected TaskMap _tasks` 





#### `protected TriggerMap _triggers` 





# struct `scy::sked::Trigger` 

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
`public `[`DateTime`](#classscy_1_1DateTime)` createdAt` | The time the task was created.
`public `[`DateTime`](#classscy_1_1DateTime)` scheduleAt` | The time the task is scheduled to run.
`public `[`DateTime`](#classscy_1_1DateTime)` lastRunAt` | The time the task was last run.
`public  Trigger(const std::string & type,const std::string & name)` | 
`public void update()` | 
`public virtual std::int64_t remaining()` | 
`public virtual bool timeout()` | 
`public virtual bool expired()` | 
`public virtual void serialize(json::Value & root)` | 
`public virtual void deserialize(json::Value & root)` | 

## Members

#### `public std::string type` 

The type of this trigger class.



#### `public std::string name` 

The display name of this trigger class.



#### `public int timesRun` 



The number of times the task has run since creation;

#### `public `[`DateTime`](#classscy_1_1DateTime)` createdAt` 

The time the task was created.



#### `public `[`DateTime`](#classscy_1_1DateTime)` scheduleAt` 

The time the task is scheduled to run.



#### `public `[`DateTime`](#classscy_1_1DateTime)` lastRunAt` 

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

#### `public virtual void serialize(json::Value & root)` 





#### `public virtual void deserialize(json::Value & root)` 





# struct `scy::sked::OnceOnlyTrigger` 

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

#### `public  OnceOnlyTrigger()` 





#### `public inline virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` 



Returns true if the task is expired and should be destroyed. Returns false by default.

# struct `scy::sked::IntervalTrigger` 

```
struct scy::sked::IntervalTrigger
  : public scy::sked::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`Timespan`](#classscy_1_1Timespan)` interval` | 
`public int maxTimes` | 
`public  IntervalTrigger()` | 
`public virtual void update()` | 
`public virtual bool expired()` | 
`public virtual void serialize(json::Value & root)` | 
`public virtual void deserialize(json::Value & root)` | 

## Members

#### `public `[`Timespan`](#classscy_1_1Timespan)` interval` 



This value represents the interval to wait before running the task.

#### `public int maxTimes` 



The maximum number of times the task will be run before it is destroyed. 0 for no effect.

#### `public  IntervalTrigger()` 





#### `public virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

#### `public virtual bool expired()` 



Returns true if the task is expired and should be destroyed. Returns false by default.

#### `public virtual void serialize(json::Value & root)` 





#### `public virtual void deserialize(json::Value & root)` 





# struct `scy::sked::DailyTrigger` 

```
struct scy::sked::DailyTrigger
  : public scy::sked::Trigger
```  





## Summary

 Members                        | Descriptions                                
--------------------------------|---------------------------------------------
`public `[`DateTime`](#classscy_1_1DateTime)` timeOfDay` | 
`public std::vector< `[`DaysOfTheWeek`](#group__sked_1gafaf09924bf11ff91312d278879efc364)` > daysExcluded` | 
`public  DailyTrigger()` | 
`public virtual void update()` | 

## Members

#### `public `[`DateTime`](#classscy_1_1DateTime)` timeOfDay` 



This value represents the time of day the task will trigger. The date part of the timestamp is redundant.

#### `public std::vector< `[`DaysOfTheWeek`](#group__sked_1gafaf09924bf11ff91312d278879efc364)` > daysExcluded` 



Days of the week can be excluded by adding the appropriate bit flag here.

#### `public  DailyTrigger()` 





#### `public virtual void update()` 



Updates the scheduleAt value to the next scheduled time.

