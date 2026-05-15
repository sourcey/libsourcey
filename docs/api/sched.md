{#schedmodule}

# sched

Task scheduler for deferred and periodic jobs.

### Namespaces

| Name | Description |
|------|-------------|
| [`sched`](#sched) | Deferred and periodic job scheduling primitives. |

{#sched}

# sched

Deferred and periodic job scheduling primitives.

### Classes

| Name | Description |
|------|-------------|
| [`Scheduler`](#scheduler) | The [Scheduler](#scheduler) manages and runs tasks that need to be executed at specific times. |
| [`Task`](#task-1) | Scheduled task with an attached trigger and scheduler association. |
| [`TaskFactory`](#taskfactory) | The [TaskFactory](#taskfactory) can dynamically instantiate registered [sched::Task](#task-1) and [sched::Trigger](#trigger-1) classes from named strings. |
| [`DailyTrigger`](#dailytrigger) | [Trigger](#trigger-1) that fires once per day at a configured time, with optional day-of-week exclusions. |
| [`IntervalTrigger`](#intervaltrigger) | [Trigger](#trigger-1) that fires repeatedly at a fixed time interval. |
| [`OnceOnlyTrigger`](#onceonlytrigger) | [Trigger](#trigger-1) that fires exactly once at the scheduled time and then expires. |
| [`Trigger`](#trigger-1) | Base class for scheduled task triggers that determine when a task should run. |

### Enumerations

| Name | Description |
|------|-------------|
| [`DaysOfTheWeek`](#daysoftheweek)  | Days of the week. |
| [`MonthOfTheYeay`](#monthoftheyeay)  | Months of the year. |

---

{#daysoftheweek}

#### DaysOfTheWeek

```cpp
enum DaysOfTheWeek
```

Days of the week.

| Value | Description |
|-------|-------------|
| `Sunday` |  |
| `Monday` |  |
| `Tuesday` |  |
| `Wednesday` |  |
| `Thursday` |  |
| `Friday` |  |
| `Saturday` |  |

---

{#monthoftheyeay}

#### MonthOfTheYeay

```cpp
enum MonthOfTheYeay
```

Months of the year.

| Value | Description |
|-------|-------------|
| `January` |  |
| `February` |  |
| `March` |  |
| `April` |  |
| `May` |  |
| `June` |  |
| `July` |  |
| `August` |  |
| `September` |  |
| `October` |  |
| `November` |  |
| `December` |  |

### Typedefs

| Return | Name | Description |
|--------|------|-------------|
| `std::vector< sched::Task * >` | [`TaskList`](#tasklist)  | Ordered list of task pointers used by the scheduler. |

---

{#tasklist}

#### TaskList

```cpp
using TaskList = std::vector< sched::Task * >
```

Ordered list of task pointers used by the scheduler.

### Functions

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< sched::Task >` | [`instantiateTask`](#instantiatetask)  | Instantiate a registered [Task](#task-1) subtype using its default constructor. |
| `std::unique_ptr< sched::Trigger >` | [`instantiateTrigger`](#instantiatetrigger)  | Instantiate a registered [Trigger](#trigger-1) subtype using its default constructor. |

---

{#instantiatetask}

#### instantiateTask

```cpp
template<typename T> std::unique_ptr< sched::Task > instantiateTask()
```

Instantiate a registered [Task](#task-1) subtype using its default constructor.

---

{#instantiatetrigger}

#### instantiateTrigger

```cpp
template<typename T> std::unique_ptr< sched::Trigger > instantiateTrigger()
```

Instantiate a registered [Trigger](#trigger-1) subtype using its default constructor.

{#scheduler}

## Scheduler

```cpp
#include <icy/sched/scheduler.h>
```

```cpp
class Scheduler
```

Defined in src/sched/include/icy/sched/scheduler.h:31

> **Inherits:** [`TaskRunner`](base.md#taskrunner), [`ISerializable`](json.md#iserializable)

The [Scheduler](#scheduler) manages and runs tasks that need to be executed at specific times.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Scheduler`](#scheduler-1) | `function` | Declared here |
| [`schedule`](#schedule) | `function` | Declared here |
| [`cancel`](#cancel-4) | `function` | Declared here |
| [`clear`](#clear-2) | `function` | Declared here |
| [`serialize`](#serialize-3) | `function` | Declared here |
| [`deserialize`](#deserialize-3) | `function` | Declared here |
| [`print`](#print-14) | `function` | Declared here |
| [`cancel`](#cancel-5) | `function` | Declared here |
| [`cancel`](#cancel-6) | `function` | Declared here |
| [`getDefault`](#getdefault-2) | `function` | Declared here |
| [`factory`](#factory-1) | `function` | Declared here |
| [`run`](#run-6) | `function` | Declared here |
| [`update`](#update-5) | `function` | Declared here |
| [`TaskList`](base.md#classicy_1_1TaskRunner_1ab5b34efcc94074c47bfeb15bf163e51c) | `typedef` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`Idle`](base.md#classicy_1_1TaskRunner_1a30a56b557732d674eb8c56a92a3b39bf) | `variable` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`Shutdown`](base.md#classicy_1_1TaskRunner_1a4536c462174e6dbea54ad084549b3be5) | `variable` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`_mutex`](base.md#classicy_1_1TaskRunner_1a8ed72724333410e042b847ac19daf9c4) | `variable` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`_runner`](base.md#classicy_1_1TaskRunner_1a315262f5ecce82c4db6e45063c2df1ac) | `variable` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`_tasks`](base.md#classicy_1_1TaskRunner_1a1cec7a3c24c2aece661b60d6405cd3c5) | `variable` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`TaskRunner`](base.md#classicy_1_1TaskRunner_1a9b67f07ab9746294356df14a1a860f04) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`~TaskRunner`](base.md#classicy_1_1TaskRunner_1a7c0c280f3659c5d9064ac8476b223cb2) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`TaskRunner`](base.md#classicy_1_1TaskRunner_1a7da8c31a699f676e059bb39f3e97f76c) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`operator=`](base.md#classicy_1_1TaskRunner_1a9c146e31a6df7fdf7148fd33799d1ea3) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`TaskRunner`](base.md#classicy_1_1TaskRunner_1adde5e6835fb88de26936c1cd7a724f00) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`operator=`](base.md#classicy_1_1TaskRunner_1a46409938bd8d3f0e6c46c33ac1dce823) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`start`](base.md#classicy_1_1TaskRunner_1a88082927b85c8136b10bbb6151f00d22) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`cancel`](base.md#classicy_1_1TaskRunner_1aa69462588279f04fc131be61adc98d0d) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`destroy`](base.md#classicy_1_1TaskRunner_1a6aa88f4f738cfb22d6033c2897c7208d) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`exists`](base.md#classicy_1_1TaskRunner_1a6869bb7ff601ca2ed84024091a474652) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`get`](base.md#classicy_1_1TaskRunner_1adeb64f098c412f3149c489d9df7b1109) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`setRunner`](base.md#classicy_1_1TaskRunner_1a8a6bfd5f77b9a25b59f5e0e137cd8782) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`className`](base.md#classicy_1_1TaskRunner_1ad9bf53e52d61b811ffa85bc911620a22) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`cancel`](base.md#classicy_1_1TaskRunner_1aa2f1d9a49043af08d17669dddfcdc268) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`getDefault`](base.md#classicy_1_1TaskRunner_1ae223a7b6cdefc4ff1dfb0d74785ce101) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`run`](base.md#classicy_1_1TaskRunner_1a99cef8fc7be815ddd9c151dd4d6985d2) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`add`](base.md#classicy_1_1TaskRunner_1a6ff09a58487ce9be902ac0e8415a2cca) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`remove`](base.md#classicy_1_1TaskRunner_1ae6bf3614249df759f7bcc611fec5a3e5) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`next`](base.md#classicy_1_1TaskRunner_1a766c070279f7f038ff242d8f1df81a3d) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`clear`](base.md#classicy_1_1TaskRunner_1a37a9f66c38ea632138c8d4f1951cd28c) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`onAdd`](base.md#classicy_1_1TaskRunner_1a5f720ac54054e1533bd25bbe43788756) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`onStart`](base.md#classicy_1_1TaskRunner_1a59c646e3779deb0fda66bddf02616724) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`onCancel`](base.md#classicy_1_1TaskRunner_1a36503096ac04ea774bb1e78b1ecdf0ef) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`onRemove`](base.md#classicy_1_1TaskRunner_1ac28c9029f03f161c3d5f375ee38140d4) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`onRun`](base.md#classicy_1_1TaskRunner_1a4e1e0694127e7aa909f4d6ef6b717e4c) | `function` | Inherited from [`TaskRunner`](base.md#taskrunner) |
| [`Runnable`](base.md#runnable-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`run`](base.md#run-4) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancel`](base.md#cancel-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancelled`](base.md#cancelled) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`exit`](base.md#exit) | `variable` | Inherited from [`Runnable`](base.md#runnable) |
| [`serialize`](json.md#serialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |
| [`deserialize`](json.md#deserialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |

### Inherited from [`TaskRunner`](base.md#taskrunner)

| Kind | Name | Description |
|------|------|-------------|
| `typedef` | [`TaskList`](base.md#classicy_1_1TaskRunner_1ab5b34efcc94074c47bfeb15bf163e51c)  |  |
| `variable` | [`Idle`](base.md#classicy_1_1TaskRunner_1a30a56b557732d674eb8c56a92a3b39bf)  | Fires after completing an iteration of all tasks. |
| `variable` | [`Shutdown`](base.md#classicy_1_1TaskRunner_1a4536c462174e6dbea54ad084549b3be5)  | Signals when the `[TaskRunner](base.md#taskrunner)` is shutting down. |
| `variable` | [`_mutex`](base.md#classicy_1_1TaskRunner_1a8ed72724333410e042b847ac19daf9c4)  |  |
| `variable` | [`_runner`](base.md#classicy_1_1TaskRunner_1a315262f5ecce82c4db6e45063c2df1ac)  |  |
| `variable` | [`_tasks`](base.md#classicy_1_1TaskRunner_1a1cec7a3c24c2aece661b60d6405cd3c5)  |  |
| `function` | [`TaskRunner`](base.md#classicy_1_1TaskRunner_1a9b67f07ab9746294356df14a1a860f04)  |  |
| `function` | [`~TaskRunner`](base.md#classicy_1_1TaskRunner_1a7c0c280f3659c5d9064ac8476b223cb2) `virtual` |  |
| `function` | [`TaskRunner`](base.md#classicy_1_1TaskRunner_1a7da8c31a699f676e059bb39f3e97f76c)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1TaskRunner_1a9c146e31a6df7fdf7148fd33799d1ea3)  | Deleted assignment operator. |
| `function` | [`TaskRunner`](base.md#classicy_1_1TaskRunner_1adde5e6835fb88de26936c1cd7a724f00)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1TaskRunner_1a46409938bd8d3f0e6c46c33ac1dce823)  | Deleted assignment operator. |
| `function` | [`start`](base.md#classicy_1_1TaskRunner_1a88082927b85c8136b10bbb6151f00d22) `virtual` | Starts a task, adding it if it doesn't exist. |
| `function` | [`cancel`](base.md#classicy_1_1TaskRunner_1aa69462588279f04fc131be61adc98d0d) `virtual` | Cancels a task. |
| `function` | [`destroy`](base.md#classicy_1_1TaskRunner_1a6aa88f4f738cfb22d6033c2897c7208d) `virtual` | Queues a task for destruction. |
| `function` | [`exists`](base.md#classicy_1_1TaskRunner_1a6869bb7ff601ca2ed84024091a474652) `virtual` `const` | Returns whether a task exists. |
| `function` | [`get`](base.md#classicy_1_1TaskRunner_1adeb64f098c412f3149c489d9df7b1109) `virtual` `const` | Returns the task pointer matching the given ID, or nullptr if no task exists. |
| `function` | [`setRunner`](base.md#classicy_1_1TaskRunner_1a8a6bfd5f77b9a25b59f5e0e137cd8782) `virtual` | Set the asynchronous context for packet processing. This may be a [Thread](base.md#thread) or another derivative of Async. Must be set before the stream is activated. |
| `function` | [`className`](base.md#classicy_1_1TaskRunner_1ad9bf53e52d61b811ffa85bc911620a22) `virtual` `const` `inline` |  |
| `function` | [`cancel`](base.md#classicy_1_1TaskRunner_1aa2f1d9a49043af08d17669dddfcdc268) `virtual` `inline` | Cancel the current task. The [run()](base.md#classicy_1_1TaskRunner_1a99cef8fc7be815ddd9c151dd4d6985d2) method should return ASAP. |
| `function` | [`getDefault`](base.md#classicy_1_1TaskRunner_1ae223a7b6cdefc4ff1dfb0d74785ce101) `static` | Returns the default `[TaskRunner](base.md#taskrunner)` singleton, although [TaskRunner](base.md#taskrunner) instances may be initialized individually. The default runner should be kept for short running tasks such as timers in order to maintain performance. |
| `function` | [`run`](base.md#classicy_1_1TaskRunner_1a99cef8fc7be815ddd9c151dd4d6985d2) `virtual` `override` | Called by the async context to run the next task. |
| `function` | [`add`](base.md#classicy_1_1TaskRunner_1a6ff09a58487ce9be902ac0e8415a2cca) `virtual` | Adds a task to the runner. |
| `function` | [`remove`](base.md#classicy_1_1TaskRunner_1ae6bf3614249df759f7bcc611fec5a3e5) `virtual` | Removes a task from the runner. |
| `function` | [`next`](base.md#classicy_1_1TaskRunner_1a766c070279f7f038ff242d8f1df81a3d) `virtual` `const` | Returns the next task to be run. |
| `function` | [`clear`](base.md#classicy_1_1TaskRunner_1a37a9f66c38ea632138c8d4f1951cd28c) `virtual` | Destroys and clears all manages tasks. |
| `function` | [`onAdd`](base.md#classicy_1_1TaskRunner_1a5f720ac54054e1533bd25bbe43788756) `virtual` | Called after a task is added. |
| `function` | [`onStart`](base.md#classicy_1_1TaskRunner_1a59c646e3779deb0fda66bddf02616724) `virtual` | Called after a task is started. |
| `function` | [`onCancel`](base.md#classicy_1_1TaskRunner_1a36503096ac04ea774bb1e78b1ecdf0ef) `virtual` | Called after a task is cancelled. |
| `function` | [`onRemove`](base.md#classicy_1_1TaskRunner_1ac28c9029f03f161c3d5f375ee38140d4) `virtual` | Called after a task is removed. |
| `function` | [`onRun`](base.md#classicy_1_1TaskRunner_1a4e1e0694127e7aa909f4d6ef6b717e4c) `virtual` | Called after a task has run. |

### Inherited from [`Runnable`](base.md#runnable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Runnable`](base.md#runnable-1) `inline` |  |
| `function` | [`run`](base.md#run-4) `virtual` | The run method will be called by the asynchronous context. |
| `function` | [`cancel`](base.md#cancel-1) `virtual` `inline` | Cancel the current task. The [run()](base.md#run-4) method should return ASAP. |
| `function` | [`cancelled`](base.md#cancelled) `virtual` `const` `inline` | Returns true when the task has been cancelled. |
| `variable` | [`exit`](base.md#exit)  |  |

### Inherited from [`ISerializable`](json.md#iserializable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`serialize`](json.md#serialize-1) `virtual` | Serializes this object's state into `root`. |
| `function` | [`deserialize`](json.md#deserialize-1) `virtual` | Populates this object's state from `root`. |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Scheduler`](#scheduler-1)  |  |
| `void` | [`schedule`](#schedule) `virtual` | Adds `task` to the scheduler and starts running it on its configured trigger. The scheduler takes ownership of the task. |
| `void` | [`cancel`](#cancel-4) `virtual` | Removes `task` from the scheduler and cancels any pending execution. |
| `void` | [`clear`](#clear-2) `virtual` `override` | Removes all scheduled tasks. |
| `void` | [`serialize`](#serialize-3) `virtual` `override` | Serializes all scheduled tasks and their triggers to `root`. |
| `void` | [`deserialize`](#deserialize-3) `virtual` `override` | Reconstructs the task list from `root` using the [TaskFactory](#taskfactory). Skips entries that fail to deserialize and logs the error. |
| `void` | [`print`](#print-14) `virtual` | Writes a pretty-printed JSON representation of all tasks to `ost`. |
| `bool` | [`cancel`](#cancel-5) `virtual` | Cancels a task. |
| `void` | [`cancel`](#cancel-6) `virtual` `inline` | Cancel the current task. The [run()](#run-6) method should return ASAP. |

---

{#scheduler-1}

#### Scheduler

```cpp
Scheduler()
```

Defined in src/sched/include/icy/sched/scheduler.h:35

---

{#schedule}

#### schedule

`virtual`

```cpp
virtual void schedule(sched::Task * task)
```

Defined in src/sched/include/icy/sched/scheduler.h:41

Adds `task` to the scheduler and starts running it on its configured trigger. The scheduler takes ownership of the task. 
#### Parameters
* `task` [Task](#task-1) to schedule; must have a trigger set.

---

{#cancel-4}

#### cancel

`virtual`

```cpp
virtual void cancel(sched::Task * task)
```

Defined in src/sched/include/icy/sched/scheduler.h:47

Removes `task` from the scheduler and cancels any pending execution. 
#### Parameters
* `task` [Task](#task-1) to cancel.

---

{#clear-2}

#### clear

`virtual` `override`

```cpp
virtual void clear() override
```

Defined in src/sched/include/icy/sched/scheduler.h:50

Removes all scheduled tasks.

##### Reimplements

- [`clear`](base.md#classicy_1_1TaskRunner_1a37a9f66c38ea632138c8d4f1951cd28c)

---

{#serialize-3}

#### serialize

`virtual` `override`

```cpp
virtual void serialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/scheduler.h:54

Serializes all scheduled tasks and their triggers to `root`. 
#### Parameters
* `root` JSON array to append serialized task entries to.

##### Reimplements

- [`serialize`](json.md#serialize-1)

---

{#deserialize-3}

#### deserialize

`virtual` `override`

```cpp
virtual void deserialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/scheduler.h:59

Reconstructs the task list from `root` using the [TaskFactory](#taskfactory). Skips entries that fail to deserialize and logs the error. 
#### Parameters
* `root` JSON array previously produced by [serialize()](#serialize-3).

##### Reimplements

- [`deserialize`](json.md#deserialize-1)

---

{#print-14}

#### print

`virtual`

```cpp
virtual void print(std::ostream & ost)
```

Defined in src/sched/include/icy/sched/scheduler.h:63

Writes a pretty-printed JSON representation of all tasks to `ost`. 
#### Parameters
* `ost` Output stream to write to.

---

{#cancel-5}

#### cancel

`virtual`

```cpp
virtual bool cancel(Task * task)
```

Defined in src/sched/include/icy/sched/scheduler.h:43

Cancels a task.

The task reference will be managed by the [TaskRunner](base.md#taskrunner) until the task is destroyed.

##### Reimplements

- [`cancel`](base.md#classicy_1_1TaskRunner_1aa69462588279f04fc131be61adc98d0d)

---

{#cancel-6}

#### cancel

`virtual` `inline`

```cpp
virtual inline void cancel(bool flag = true)
```

Defined in src/sched/include/icy/sched/scheduler.h:43

Cancel the current task. The [run()](#run-6) method should return ASAP.

##### Reimplements

- [`cancel`](base.md#cancel-1)

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `Scheduler &` | [`getDefault`](#getdefault-2) `static` | Returns the default [Scheduler](#scheduler) singleton, although [Scheduler](#scheduler) instances may also be initialized individually. |
| `sched::TaskFactory &` | [`factory`](#factory-1) `static` | Returns the [TaskFactory](#taskfactory) singleton. |

---

{#getdefault-2}

#### getDefault

`static`

```cpp
static Scheduler & getDefault()
```

Defined in src/sched/include/icy/sched/scheduler.h:68

Returns the default [Scheduler](#scheduler) singleton, although [Scheduler](#scheduler) instances may also be initialized individually.

---

{#factory-1}

#### factory

`static`

```cpp
static sched::TaskFactory & factory()
```

Defined in src/sched/include/icy/sched/scheduler.h:71

Returns the [TaskFactory](#taskfactory) singleton.

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `void` | [`run`](#run-6) `virtual` `override` | The run method will be called by the asynchronous context. |
| `void` | [`update`](#update-5) `virtual` |  |

---

{#run-6}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/sched/include/icy/sched/scheduler.h:74

The run method will be called by the asynchronous context.

##### Reimplements

- [`run`](base.md#run-4)

---

{#update-5}

#### update

`virtual`

```cpp
virtual void update()
```

Defined in src/sched/include/icy/sched/scheduler.h:75

{#task-1}

## Task

```cpp
#include <icy/sched/task.h>
```

```cpp
class Task
```

Defined in src/sched/include/icy/sched/task.h:32

> **Inherits:** [`Task`](base.md#task), [`ISerializable`](json.md#iserializable)

Scheduled task with an attached trigger and scheduler association.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`Scheduler`](#scheduler-2) | `friend` | Declared here |
| [`Task`](#task-2) | `function` | Declared here |
| [`Task`](#task-3) | `function` | Declared here |
| [`serialize`](#serialize-4) | `function` | Declared here |
| [`deserialize`](#deserialize-4) | `function` | Declared here |
| [`createTrigger`](#createtrigger) | `function` | Declared here |
| [`setTrigger`](#settrigger) | `function` | Declared here |
| [`trigger`](#trigger) | `function` | Declared here |
| [`scheduler`](#scheduler-3) | `function` | Declared here |
| [`remaining`](#remaining) | `function` | Declared here |
| [`type`](#type-16) | `function` | Declared here |
| [`name`](#name-9) | `function` | Declared here |
| [`setName`](#setname-1) | `function` | Declared here |
| [`_type`](#_type-2) | `variable` | Declared here |
| [`_name`](#_name-1) | `variable` | Declared here |
| [`_scheduler`](#_scheduler) | `variable` | Declared here |
| [`_trigger`](#_trigger) | `variable` | Declared here |
| [`_mutex`](#_mutex-14) | `variable` | Declared here |
| [`beforeRun`](#beforerun) | `function` | Declared here |
| [`run`](#run-7) | `function` | Declared here |
| [`afterRun`](#afterrun) | `function` | Declared here |
| [`TaskRunner`](base.md#classicy_1_1Task_1a1956d94b45592a4096a335641aab65f8) | `friend` | Inherited from [`Task`](base.md#task) |
| [`_id`](base.md#classicy_1_1Task_1ac1ee918bfc4a8bef0dd1661f4cbe680a) | `variable` | Inherited from [`Task`](base.md#task) |
| [`_repeating`](base.md#classicy_1_1Task_1a348904116c7dd4d3f415452681599b0e) | `variable` | Inherited from [`Task`](base.md#task) |
| [`_destroyed`](base.md#classicy_1_1Task_1aa0692b25828d7a0da407a17d137ff782) | `variable` | Inherited from [`Task`](base.md#task) |
| [`Task`](base.md#classicy_1_1Task_1aa74fdf87d9f64104eef70e879ea6bd27) | `function` | Inherited from [`Task`](base.md#task) |
| [`destroy`](base.md#classicy_1_1Task_1a639abe7ed51fd9294d6210e1d8264814) | `function` | Inherited from [`Task`](base.md#task) |
| [`destroyed`](base.md#classicy_1_1Task_1acee30db8dbcdc6a7508e3d407b923cce) | `function` | Inherited from [`Task`](base.md#task) |
| [`repeating`](base.md#classicy_1_1Task_1ac5c935259f0e714c209834faad6ef106) | `function` | Inherited from [`Task`](base.md#task) |
| [`id`](base.md#classicy_1_1Task_1a763945e1432e4db22ed5dbcb9682fadb) | `function` | Inherited from [`Task`](base.md#task) |
| [`~Task`](base.md#classicy_1_1Task_1a3844bd8461c7b8d295c5807fe9ae8b2d) | `function` | Inherited from [`Task`](base.md#task) |
| [`Task`](base.md#classicy_1_1Task_1acbb62d60d08febc0f2eb088aaf938116) | `function` | Inherited from [`Task`](base.md#task) |
| [`operator=`](base.md#classicy_1_1Task_1a0239870fcbb01195d6d53c7ef83a7c68) | `function` | Inherited from [`Task`](base.md#task) |
| [`Task`](base.md#classicy_1_1Task_1a9df1005fc4dafee44be6246cd03cb8c6) | `function` | Inherited from [`Task`](base.md#task) |
| [`operator=`](base.md#classicy_1_1Task_1a89132b582d3c178b01e4c102aac03f9a) | `function` | Inherited from [`Task`](base.md#task) |
| [`run`](base.md#classicy_1_1Task_1a27c42c43ba0ce7ea66f3210fdd27f3de) | `function` | Inherited from [`Task`](base.md#task) |
| [`Runnable`](base.md#runnable-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`run`](base.md#run-4) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancel`](base.md#cancel-1) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`cancelled`](base.md#cancelled) | `function` | Inherited from [`Runnable`](base.md#runnable) |
| [`exit`](base.md#exit) | `variable` | Inherited from [`Runnable`](base.md#runnable) |
| [`serialize`](json.md#serialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |
| [`deserialize`](json.md#deserialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |

### Inherited from [`Task`](base.md#task)

| Kind | Name | Description |
|------|------|-------------|
| `friend` | [`TaskRunner`](base.md#classicy_1_1Task_1a1956d94b45592a4096a335641aab65f8)  | Tasks belong to a [TaskRunner](base.md#taskrunner) instance. |
| `variable` | [`_id`](base.md#classicy_1_1Task_1ac1ee918bfc4a8bef0dd1661f4cbe680a)  |  |
| `variable` | [`_repeating`](base.md#classicy_1_1Task_1a348904116c7dd4d3f415452681599b0e)  |  |
| `variable` | [`_destroyed`](base.md#classicy_1_1Task_1aa0692b25828d7a0da407a17d137ff782)  |  |
| `function` | [`Task`](base.md#classicy_1_1Task_1aa74fdf87d9f64104eef70e879ea6bd27)  |  |
| `function` | [`destroy`](base.md#classicy_1_1Task_1a639abe7ed51fd9294d6210e1d8264814) `virtual` | Sets the task to destroyed state. |
| `function` | [`destroyed`](base.md#classicy_1_1Task_1acee30db8dbcdc6a7508e3d407b923cce) `virtual` `const` | Signals that the task should be disposed of. |
| `function` | [`repeating`](base.md#classicy_1_1Task_1ac5c935259f0e714c209834faad6ef106) `virtual` `const` | Signals that the task should be called repeatedly by the [TaskRunner](base.md#taskrunner). If this returns false the task will be cancelled. |
| `function` | [`id`](base.md#classicy_1_1Task_1a763945e1432e4db22ed5dbcb9682fadb) `virtual` `const` | Unique task ID. |
| `function` | [`~Task`](base.md#classicy_1_1Task_1a3844bd8461c7b8d295c5807fe9ae8b2d) `virtual` |  |
| `function` | [`Task`](base.md#classicy_1_1Task_1acbb62d60d08febc0f2eb088aaf938116)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1Task_1a0239870fcbb01195d6d53c7ef83a7c68)  | Deleted assignment operator. |
| `function` | [`Task`](base.md#classicy_1_1Task_1a9df1005fc4dafee44be6246cd03cb8c6)  | Deleted constructor. |
| `function` | [`operator=`](base.md#classicy_1_1Task_1a89132b582d3c178b01e4c102aac03f9a)  | Deleted assignment operator. |
| `function` | [`run`](base.md#classicy_1_1Task_1a27c42c43ba0ce7ea66f3210fdd27f3de) `virtual` `override` | Called by the [TaskRunner](base.md#taskrunner) to run the task. Override this method to implement task action. Returning true means the task should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed if [destroy()](base.md#classicy_1_1Task_1a639abe7ed51fd9294d6210e1d8264814) was called during the current task iteration. |

### Inherited from [`Runnable`](base.md#runnable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`Runnable`](base.md#runnable-1) `inline` |  |
| `function` | [`run`](base.md#run-4) `virtual` | The run method will be called by the asynchronous context. |
| `function` | [`cancel`](base.md#cancel-1) `virtual` `inline` | Cancel the current task. The [run()](base.md#run-4) method should return ASAP. |
| `function` | [`cancelled`](base.md#cancelled) `virtual` `const` `inline` | Returns true when the task has been cancelled. |
| `variable` | [`exit`](base.md#exit)  |  |

### Inherited from [`ISerializable`](json.md#iserializable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`serialize`](json.md#serialize-1) `virtual` | Serializes this object's state into `root`. |
| `function` | [`deserialize`](json.md#deserialize-1) `virtual` | Populates this object's state from `root`. |

### Friends

| Name | Description |
|------|-------------|
| [`Scheduler`](#scheduler-2)  |  |

---

{#scheduler-2}

#### Scheduler

```cpp
friend class Scheduler
```

Defined in src/sched/include/icy/sched/task.h:111

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Task`](#task-2)  | Constructs a detached task without an associated scheduler. A trigger must be set before scheduling. |
|  | [`Task`](#task-3)  | Constructs a task associated with the given scheduler. |
| `void` | [`serialize`](#serialize-4) `virtual` `override` | Serializes the task to JSON. |
| `void` | [`deserialize`](#deserialize-4) `virtual` `override` | Deserializes the task from JSON. |
| `T *` | [`createTrigger`](#createtrigger) `inline` | Creates a trigger of type T, attaches it to this task, and returns a raw pointer to it. Ownership of the trigger is transferred to this task. |
| `void` | [`setTrigger`](#settrigger)  | Replaces the current trigger with `trigger`. |
| `sched::Trigger &` | [`trigger`](#trigger)  | Returns a reference to the associated [sched::Trigger](#trigger-1) or throws an exception. |
| `Scheduler &` | [`scheduler`](#scheduler-3)  | Returns a reference to the associated [Scheduler](#scheduler) or throws an exception. |
| `std::int64_t` | [`remaining`](#remaining) `const` | Returns the milliseconds remaining until the next scheduled timeout. An [sched::Trigger](#trigger-1) must be associated or an exception will be thrown. |
| `std::string` | [`type`](#type-16) `const` | Returns the registered type string for this task. |
| `std::string` | [`name`](#name-9) `const` | Returns the human-readable display name of this task. |
| `void` | [`setName`](#setname-1)  | Sets the human-readable display name. |

---

{#task-2}

#### Task

```cpp
Task(const std::string & type = "", const std::string & name = "")
```

Defined in src/sched/include/icy/sched/task.h:40

Constructs a detached task without an associated scheduler. A trigger must be set before scheduling. 
#### Parameters
* `type` Registered type name used by [TaskFactory](#taskfactory). 

* `name` Human-readable display name.

---

{#task-3}

#### Task

```cpp
Task(Scheduler & scheduler, const std::string & type, const std::string & name = "")
```

Defined in src/sched/include/icy/sched/task.h:46

Constructs a task associated with the given scheduler. 
#### Parameters
* `scheduler` [Scheduler](#scheduler) that will own and run this task. 

* `type` Registered type name used by [TaskFactory](#taskfactory). 

* `name` Human-readable display name.

---

{#serialize-4}

#### serialize

`virtual` `override`

```cpp
virtual void serialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/task.h:51

Serializes the task to JSON.

##### Reimplements

- [`serialize`](json.md#serialize-1)

---

{#deserialize-4}

#### deserialize

`virtual` `override`

```cpp
virtual void deserialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/task.h:54

Deserializes the task from JSON.

##### Reimplements

- [`deserialize`](json.md#deserialize-1)

---

{#createtrigger}

#### createTrigger

`inline`

```cpp
template<typename T> inline T * createTrigger()
```

Defined in src/sched/include/icy/sched/task.h:61

Creates a trigger of type T, attaches it to this task, and returns a raw pointer to it. Ownership of the trigger is transferred to this task. 
#### Parameters
* `T` A concrete subclass of [sched::Trigger](#trigger-1). 

#### Returns
Raw pointer to the newly created trigger (still owned by this task).

---

{#settrigger}

#### setTrigger

```cpp
void setTrigger(std::unique_ptr< sched::Trigger > trigger)
```

Defined in src/sched/include/icy/sched/task.h:71

Replaces the current trigger with `trigger`. 
#### Parameters
* `trigger` Owning pointer to the new trigger; must not be null before calling.

---

{#trigger}

#### trigger

```cpp
sched::Trigger & trigger()
```

Defined in src/sched/include/icy/sched/task.h:75

Returns a reference to the associated [sched::Trigger](#trigger-1) or throws an exception.

---

{#scheduler-3}

#### scheduler

```cpp
Scheduler & scheduler()
```

Defined in src/sched/include/icy/sched/task.h:79

Returns a reference to the associated [Scheduler](#scheduler) or throws an exception.

---

{#remaining}

#### remaining

`const`

```cpp
std::int64_t remaining() const
```

Defined in src/sched/include/icy/sched/task.h:85

Returns the milliseconds remaining until the next scheduled timeout. An [sched::Trigger](#trigger-1) must be associated or an exception will be thrown.

---

{#type-16}

#### type

`const`

```cpp
std::string type() const
```

Defined in src/sched/include/icy/sched/task.h:88

Returns the registered type string for this task.

---

{#name-9}

#### name

`const`

```cpp
std::string name() const
```

Defined in src/sched/include/icy/sched/task.h:91

Returns the human-readable display name of this task.

---

{#setname-1}

#### setName

```cpp
void setName(const std::string & name)
```

Defined in src/sched/include/icy/sched/task.h:95

Sets the human-readable display name. 
#### Parameters
* `name` New display name.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`_type`](#_type-2)  |  |
| `std::string` | [`_name`](#_name-1)  |  |
| `sched::Scheduler *` | [`_scheduler`](#_scheduler)  |  |
| `std::unique_ptr< sched::Trigger >` | [`_trigger`](#_trigger)  |  |
| `std::mutex` | [`_mutex`](#_mutex-14)  |  |

---

{#_type-2}

#### _type

```cpp
std::string _type
```

Defined in src/sched/include/icy/sched/task.h:113

---

{#_name-1}

#### _name

```cpp
std::string _name
```

Defined in src/sched/include/icy/sched/task.h:114

---

{#_scheduler}

#### _scheduler

```cpp
sched::Scheduler * _scheduler
```

Defined in src/sched/include/icy/sched/task.h:115

---

{#_trigger}

#### _trigger

```cpp
std::unique_ptr< sched::Trigger > _trigger
```

Defined in src/sched/include/icy/sched/task.h:116

---

{#_mutex-14}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/sched/include/icy/sched/task.h:117

### Protected Methods

| Return | Name | Description |
|--------|------|-------------|
| `bool` | [`beforeRun`](#beforerun) `virtual` |  |
| `void` | [`run`](#run-7) `virtual` `override` | Called by the [TaskRunner](base.md#taskrunner) to run the task. Override this method to implement task action. Returning true means the task should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed if [destroy()](base.md#classicy_1_1Task_1a639abe7ed51fd9294d6210e1d8264814) was called during the current task iteration. |
| `bool` | [`afterRun`](#afterrun) `virtual` |  |

---

{#beforerun}

#### beforeRun

`virtual`

```cpp
virtual bool beforeRun()
```

Defined in src/sched/include/icy/sched/task.h:100

---

{#run-7}

#### run

`virtual` `override`

```cpp
virtual void run() override
```

Defined in src/sched/include/icy/sched/task.h:101

Called by the [TaskRunner](base.md#taskrunner) to run the task. Override this method to implement task action. Returning true means the task should be called again, and false will cause the task to be destroyed. The task will similarly be destroyed if [destroy()](base.md#classicy_1_1Task_1a639abe7ed51fd9294d6210e1d8264814) was called during the current task iteration.

##### Reimplements

- [`run`](base.md#classicy_1_1Task_1a27c42c43ba0ce7ea66f3210fdd27f3de)

---

{#afterrun}

#### afterRun

`virtual`

```cpp
virtual bool afterRun()
```

Defined in src/sched/include/icy/sched/task.h:102

{#taskfactory}

## TaskFactory

```cpp
#include <icy/sched/taskfactory.h>
```

```cpp
class TaskFactory
```

Defined in src/sched/include/icy/sched/taskfactory.h:49

The [TaskFactory](#taskfactory) can dynamically instantiate registered [sched::Task](#task-1) and [sched::Trigger](#trigger-1) classes from named strings.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`createTask`](#createtask) | `function` | Declared here |
| [`registerTask`](#registertask) | `function` | Declared here |
| [`unregisterTask`](#unregistertask) | `function` | Declared here |
| [`tasks`](#tasks-2) | `function` | Declared here |
| [`createTrigger`](#createtrigger-1) | `function` | Declared here |
| [`registerTrigger`](#registertrigger) | `function` | Declared here |
| [`unregisterTrigger`](#unregistertrigger) | `function` | Declared here |
| [`triggers`](#triggers) | `function` | Declared here |
| [`getDefault`](#getdefault-3) | `function` | Declared here |
| [`_mutex`](#_mutex-15) | `variable` | Declared here |
| [`_tasks`](#_tasks-2) | `variable` | Declared here |
| [`_triggers`](#_triggers) | `variable` | Declared here |
| [`TaskMap`](#taskmap) | `typedef` | Declared here |
| [`TriggerMap`](#triggermap) | `typedef` | Declared here |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
| `std::unique_ptr< sched::Task >` | [`createTask`](#createtask) `inline` | Instantiates and returns a registered task by type name. |
| `void` | [`registerTask`](#registertask) `inline` | Registers a task type T under the given name. Subsequent calls to [createTask()](#createtask) with this `type` will return a T instance. |
| `void` | [`unregisterTask`](#unregistertask) `inline` | Removes the task registration for `type`. No-op if not registered. |
| `TaskMap` | [`tasks`](#tasks-2) `const` `inline` | Returns a snapshot copy of the registered task map. |
| `std::unique_ptr< sched::Trigger >` | [`createTrigger`](#createtrigger-1) `inline` | Instantiates and returns a registered trigger by type name. |
| `void` | [`registerTrigger`](#registertrigger) `inline` | Registers a trigger type T under the given name. |
| `void` | [`unregisterTrigger`](#unregistertrigger) `inline` | Removes the trigger registration for `type`. No-op if not registered. |
| `TriggerMap` | [`triggers`](#triggers) `const` `inline` | Returns a snapshot copy of the registered trigger map. |

---

{#createtask}

#### createTask

`inline`

```cpp
inline std::unique_ptr< sched::Task > createTask(const std::string & type)
```

Defined in src/sched/include/icy/sched/taskfactory.h:67

Instantiates and returns a registered task by type name. 
#### Parameters
* `type` Registered type name. 

#### Returns
Owning pointer to the new task instance. 

#### Exceptions
* `std::runtime_error` if `type` is not registered.

---

{#registertask}

#### registerTask

`inline`

```cpp
template<typename T> inline void registerTask(const std::string & type)
```

Defined in src/sched/include/icy/sched/taskfactory.h:81

Registers a task type T under the given name. Subsequent calls to [createTask()](#createtask) with this `type` will return a T instance. 
#### Parameters
* `T` Concrete subclass of [sched::Task](#task-1) with a default constructor. 

#### Parameters
* `type` Type name string to register.

---

{#unregistertask}

#### unregisterTask

`inline`

```cpp
inline void unregisterTask(const std::string & type)
```

Defined in src/sched/include/icy/sched/taskfactory.h:89

Removes the task registration for `type`. No-op if not registered. 
#### Parameters
* `type` Type name to deregister.

---

{#tasks-2}

#### tasks

`const` `inline`

```cpp
inline TaskMap tasks() const
```

Defined in src/sched/include/icy/sched/taskfactory.h:99

Returns a snapshot copy of the registered task map.

---

{#createtrigger-1}

#### createTrigger

`inline`

```cpp
inline std::unique_ptr< sched::Trigger > createTrigger(const std::string & type)
```

Defined in src/sched/include/icy/sched/taskfactory.h:114

Instantiates and returns a registered trigger by type name. 
#### Parameters
* `type` Registered type name. 

#### Returns
Owning pointer to the new trigger instance. 

#### Exceptions
* `std::runtime_error` if `type` is not registered.

---

{#registertrigger}

#### registerTrigger

`inline`

```cpp
template<typename T> inline void registerTrigger(const std::string & type)
```

Defined in src/sched/include/icy/sched/taskfactory.h:127

Registers a trigger type T under the given name. 
#### Parameters
* `T` Concrete subclass of [sched::Trigger](#trigger-1) with a default constructor. 

#### Parameters
* `type` Type name string to register.

---

{#unregistertrigger}

#### unregisterTrigger

`inline`

```cpp
inline void unregisterTrigger(const std::string & type)
```

Defined in src/sched/include/icy/sched/taskfactory.h:135

Removes the trigger registration for `type`. No-op if not registered. 
#### Parameters
* `type` Type name to deregister.

---

{#triggers}

#### triggers

`const` `inline`

```cpp
inline TriggerMap triggers() const
```

Defined in src/sched/include/icy/sched/taskfactory.h:145

Returns a snapshot copy of the registered trigger map.

### Public Static Methods

| Return | Name | Description |
|--------|------|-------------|
| `TaskFactory &` | [`getDefault`](#getdefault-3) `static` `inline` | Returns the default [TaskFactory](#taskfactory) singleton. |

---

{#getdefault-3}

#### getDefault

`static` `inline`

```cpp
static inline TaskFactory & getDefault()
```

Defined in src/sched/include/icy/sched/taskfactory.h:53

Returns the default [TaskFactory](#taskfactory) singleton.

### Protected Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::mutex` | [`_mutex`](#_mutex-15)  |  |
| `TaskMap` | [`_tasks`](#_tasks-2)  |  |
| `TriggerMap` | [`_triggers`](#_triggers)  |  |

---

{#_mutex-15}

#### _mutex

```cpp
std::mutex _mutex
```

Defined in src/sched/include/icy/sched/taskfactory.h:152

---

{#_tasks-2}

#### _tasks

```cpp
TaskMap _tasks
```

Defined in src/sched/include/icy/sched/taskfactory.h:154

---

{#_triggers}

#### _triggers

```cpp
TriggerMap _triggers
```

Defined in src/sched/include/icy/sched/taskfactory.h:155

### Public Types

| Name | Description |
|------|-------------|
| [`TaskMap`](#taskmap)  | Scheduled Tasks. |
| [`TriggerMap`](#triggermap)  | Schedule Triggers. |

---

{#taskmap}

#### TaskMap

```cpp
using TaskMap = std::map< std::string, std::unique_ptr< sched::Task >(*)()>
```

Defined in src/sched/include/icy/sched/taskfactory.h:61

Scheduled Tasks.

---

{#triggermap}

#### TriggerMap

```cpp
using TriggerMap = std::map< std::string, std::unique_ptr< sched::Trigger >(*)()>
```

Defined in src/sched/include/icy/sched/taskfactory.h:108

Schedule Triggers.

{#dailytrigger}

## DailyTrigger

```cpp
#include <icy/sched/trigger.h>
```

```cpp
struct DailyTrigger
```

Defined in src/sched/include/icy/sched/trigger.h:167

> **Inherits:** [`Trigger`](#trigger-1)

[Trigger](#trigger-1) that fires once per day at a configured time, with optional day-of-week exclusions.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`timeOfDay`](#timeofday) | `variable` | Declared here |
| [`daysExcluded`](#daysexcluded) | `variable` | Declared here |
| [`DailyTrigger`](#dailytrigger-1) | `function` | Declared here |
| [`update`](#update-2) | `function` | Declared here |
| [`type`](#type-17) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`name`](#name-10) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`timesRun`](#timesrun) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`createdAt`](#createdat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`scheduleAt`](#scheduleat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`lastRunAt`](#lastrunat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`Trigger`](#trigger-2) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`update`](#update-6) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`remaining`](#remaining-1) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`timeout`](#timeout-4) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`expired`](#expired-4) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`serialize`](#serialize-5) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`deserialize`](#deserialize-5) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`serialize`](json.md#serialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |
| [`deserialize`](json.md#deserialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |

### Inherited from [`Trigger`](#trigger-1)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`type`](#type-17)  | The type of this trigger class. |
| `variable` | [`name`](#name-10)  | The display name of this trigger class. |
| `variable` | [`timesRun`](#timesrun)  | The number of times the task has run since creation; |
| `variable` | [`createdAt`](#createdat)  | The time the task was created. |
| `variable` | [`scheduleAt`](#scheduleat)  | The time the task is scheduled to run. |
| `variable` | [`lastRunAt`](#lastrunat)  | The time the task was last run. |
| `function` | [`Trigger`](#trigger-2)  |  |
| `function` | [`update`](#update-6) `virtual` | Updates the scheduleAt value to the next scheduled time. |
| `function` | [`remaining`](#remaining-1) `virtual` | Returns the milliseconds remaining until the next scheduled timeout. |
| `function` | [`timeout`](#timeout-4) `virtual` | Returns true if the task is ready to be run, false otherwise. |
| `function` | [`expired`](#expired-4) `virtual` | Returns true if the task is expired and should be destroyed. Returns false by default. |
| `function` | [`serialize`](#serialize-5) `virtual` `override` | Serializes timing state (type, name, createdAt, scheduleAt, lastRunAt, timesRun) to `root`. |
| `function` | [`deserialize`](#deserialize-5) `virtual` `override` | Deserializes timing state from `root`. |

### Inherited from [`ISerializable`](json.md#iserializable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`serialize`](json.md#serialize-1) `virtual` | Serializes this object's state into `root`. |
| `function` | [`deserialize`](json.md#deserialize-1) `virtual` | Populates this object's state from `root`. |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `DateTime` | [`timeOfDay`](#timeofday)  | This value represents the time of day the task will trigger. The date part of the timestamp is redundant. |
| `std::vector< DaysOfTheWeek >` | [`daysExcluded`](#daysexcluded)  | Days of the week can be excluded by adding the appropriate bit flag here. |

---

{#timeofday}

#### timeOfDay

```cpp
DateTime timeOfDay
```

Defined in src/sched/include/icy/sched/trigger.h:179

This value represents the time of day the task will trigger. The date part of the timestamp is redundant.

---

{#daysexcluded}

#### daysExcluded

```cpp
std::vector< DaysOfTheWeek > daysExcluded
```

Defined in src/sched/include/icy/sched/trigger.h:183

Days of the week can be excluded by adding the appropriate bit flag here.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`DailyTrigger`](#dailytrigger-1)  | Constructs the trigger with type "DailyTrigger". |
| `void` | [`update`](#update-2) `virtual` `override` | Computes the next scheduleAt value by advancing to the next non-excluded weekday at the configured timeOfDay. |

---

{#dailytrigger-1}

#### DailyTrigger

```cpp
DailyTrigger()
```

Defined in src/sched/include/icy/sched/trigger.h:170

Constructs the trigger with type "DailyTrigger".

---

{#update-2}

#### update

`virtual` `override`

```cpp
virtual void update() override
```

Defined in src/sched/include/icy/sched/trigger.h:174

Computes the next scheduleAt value by advancing to the next non-excluded weekday at the configured timeOfDay.

##### Reimplements

- [`update`](#update-6)

{#intervaltrigger}

## IntervalTrigger

```cpp
#include <icy/sched/trigger.h>
```

```cpp
struct IntervalTrigger
```

Defined in src/sched/include/icy/sched/trigger.h:133

> **Inherits:** [`Trigger`](#trigger-1)

[Trigger](#trigger-1) that fires repeatedly at a fixed time interval.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`interval`](#interval) | `variable` | Declared here |
| [`maxTimes`](#maxtimes) | `variable` | Declared here |
| [`IntervalTrigger`](#intervaltrigger-1) | `function` | Declared here |
| [`update`](#update-3) | `function` | Declared here |
| [`expired`](#expired-2) | `function` | Declared here |
| [`serialize`](#serialize-2) | `function` | Declared here |
| [`deserialize`](#deserialize-2) | `function` | Declared here |
| [`type`](#type-17) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`name`](#name-10) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`timesRun`](#timesrun) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`createdAt`](#createdat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`scheduleAt`](#scheduleat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`lastRunAt`](#lastrunat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`Trigger`](#trigger-2) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`update`](#update-6) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`remaining`](#remaining-1) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`timeout`](#timeout-4) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`expired`](#expired-4) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`serialize`](#serialize-5) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`deserialize`](#deserialize-5) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`serialize`](json.md#serialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |
| [`deserialize`](json.md#deserialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |

### Inherited from [`Trigger`](#trigger-1)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`type`](#type-17)  | The type of this trigger class. |
| `variable` | [`name`](#name-10)  | The display name of this trigger class. |
| `variable` | [`timesRun`](#timesrun)  | The number of times the task has run since creation; |
| `variable` | [`createdAt`](#createdat)  | The time the task was created. |
| `variable` | [`scheduleAt`](#scheduleat)  | The time the task is scheduled to run. |
| `variable` | [`lastRunAt`](#lastrunat)  | The time the task was last run. |
| `function` | [`Trigger`](#trigger-2)  |  |
| `function` | [`update`](#update-6) `virtual` | Updates the scheduleAt value to the next scheduled time. |
| `function` | [`remaining`](#remaining-1) `virtual` | Returns the milliseconds remaining until the next scheduled timeout. |
| `function` | [`timeout`](#timeout-4) `virtual` | Returns true if the task is ready to be run, false otherwise. |
| `function` | [`expired`](#expired-4) `virtual` | Returns true if the task is expired and should be destroyed. Returns false by default. |
| `function` | [`serialize`](#serialize-5) `virtual` `override` | Serializes timing state (type, name, createdAt, scheduleAt, lastRunAt, timesRun) to `root`. |
| `function` | [`deserialize`](#deserialize-5) `virtual` `override` | Deserializes timing state from `root`. |

### Inherited from [`ISerializable`](json.md#iserializable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`serialize`](json.md#serialize-1) `virtual` | Serializes this object's state into `root`. |
| `function` | [`deserialize`](json.md#deserialize-1) `virtual` | Populates this object's state from `root`. |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `Timespan` | [`interval`](#interval)  | This value represents the interval to wait before running the task. |
| `int` | [`maxTimes`](#maxtimes)  | The maximum number of times the task will be run before it is destroyed. 0 for no effect. |

---

{#interval}

#### interval

```cpp
Timespan interval
```

Defined in src/sched/include/icy/sched/trigger.h:155

This value represents the interval to wait before running the task.

---

{#maxtimes}

#### maxTimes

```cpp
int maxTimes
```

Defined in src/sched/include/icy/sched/trigger.h:160

The maximum number of times the task will be run before it is destroyed. 0 for no effect.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`IntervalTrigger`](#intervaltrigger-1)  | Constructs the trigger with type "IntervalTrigger" and maxTimes = 0 (unlimited). |
| `void` | [`update`](#update-3) `virtual` `override` | Advances scheduleAt by one `[interval](#interval)` period. |
| `bool` | [`expired`](#expired-2) `virtual` `override` | Returns true when maxTimes > 0 and timesRun >= maxTimes. |
| `void` | [`serialize`](#serialize-2) `virtual` `override` | Serializes interval fields (days, hours, minutes, seconds) in addition to base fields. |
| `void` | [`deserialize`](#deserialize-2) `virtual` `override` | Deserializes interval fields from `root`. Throws if the resulting interval is zero. |

---

{#intervaltrigger-1}

#### IntervalTrigger

```cpp
IntervalTrigger()
```

Defined in src/sched/include/icy/sched/trigger.h:136

Constructs the trigger with type "IntervalTrigger" and maxTimes = 0 (unlimited).

---

{#update-3}

#### update

`virtual` `override`

```cpp
virtual void update() override
```

Defined in src/sched/include/icy/sched/trigger.h:139

Advances scheduleAt by one `[interval](#interval)` period.

##### Reimplements

- [`update`](#update-6)

---

{#expired-2}

#### expired

`virtual` `override`

```cpp
virtual bool expired() override
```

Defined in src/sched/include/icy/sched/trigger.h:142

Returns true when maxTimes > 0 and timesRun >= maxTimes.

##### Reimplements

- [`expired`](#expired-4)

---

{#serialize-2}

#### serialize

`virtual` `override`

```cpp
virtual void serialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/trigger.h:146

Serializes interval fields (days, hours, minutes, seconds) in addition to base fields. 
#### Parameters
* `root` JSON object to populate.

##### Reimplements

- [`serialize`](#serialize-5)

---

{#deserialize-2}

#### deserialize

`virtual` `override`

```cpp
virtual void deserialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/trigger.h:151

Deserializes interval fields from `root`. Throws if the resulting interval is zero. 
#### Parameters
* `root` JSON object previously produced by [serialize()](#serialize-2).

##### Reimplements

- [`deserialize`](#deserialize-5)

{#onceonlytrigger}

## OnceOnlyTrigger

```cpp
#include <icy/sched/trigger.h>
```

```cpp
struct OnceOnlyTrigger
```

Defined in src/sched/include/icy/sched/trigger.h:113

> **Inherits:** [`Trigger`](#trigger-1)

[Trigger](#trigger-1) that fires exactly once at the scheduled time and then expires.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`OnceOnlyTrigger`](#onceonlytrigger-1) | `function` | Declared here |
| [`update`](#update-4) | `function` | Declared here |
| [`expired`](#expired-3) | `function` | Declared here |
| [`type`](#type-17) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`name`](#name-10) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`timesRun`](#timesrun) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`createdAt`](#createdat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`scheduleAt`](#scheduleat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`lastRunAt`](#lastrunat) | `variable` | Inherited from [`Trigger`](#trigger-1) |
| [`Trigger`](#trigger-2) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`update`](#update-6) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`remaining`](#remaining-1) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`timeout`](#timeout-4) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`expired`](#expired-4) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`serialize`](#serialize-5) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`deserialize`](#deserialize-5) | `function` | Inherited from [`Trigger`](#trigger-1) |
| [`serialize`](json.md#serialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |
| [`deserialize`](json.md#deserialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |

### Inherited from [`Trigger`](#trigger-1)

| Kind | Name | Description |
|------|------|-------------|
| `variable` | [`type`](#type-17)  | The type of this trigger class. |
| `variable` | [`name`](#name-10)  | The display name of this trigger class. |
| `variable` | [`timesRun`](#timesrun)  | The number of times the task has run since creation; |
| `variable` | [`createdAt`](#createdat)  | The time the task was created. |
| `variable` | [`scheduleAt`](#scheduleat)  | The time the task is scheduled to run. |
| `variable` | [`lastRunAt`](#lastrunat)  | The time the task was last run. |
| `function` | [`Trigger`](#trigger-2)  |  |
| `function` | [`update`](#update-6) `virtual` | Updates the scheduleAt value to the next scheduled time. |
| `function` | [`remaining`](#remaining-1) `virtual` | Returns the milliseconds remaining until the next scheduled timeout. |
| `function` | [`timeout`](#timeout-4) `virtual` | Returns true if the task is ready to be run, false otherwise. |
| `function` | [`expired`](#expired-4) `virtual` | Returns true if the task is expired and should be destroyed. Returns false by default. |
| `function` | [`serialize`](#serialize-5) `virtual` `override` | Serializes timing state (type, name, createdAt, scheduleAt, lastRunAt, timesRun) to `root`. |
| `function` | [`deserialize`](#deserialize-5) `virtual` `override` | Deserializes timing state from `root`. |

### Inherited from [`ISerializable`](json.md#iserializable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`serialize`](json.md#serialize-1) `virtual` | Serializes this object's state into `root`. |
| `function` | [`deserialize`](json.md#deserialize-1) `virtual` | Populates this object's state from `root`. |

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`OnceOnlyTrigger`](#onceonlytrigger-1)  | Constructs the trigger with type "OnceOnlyTrigger". |
| `void` | [`update`](#update-4) `virtual` `inline` `override` | No-op; scheduleAt is set once at construction and never advanced. |
| `bool` | [`expired`](#expired-3) `virtual` `override` | Returns true after the task has run at least once. |

---

{#onceonlytrigger-1}

#### OnceOnlyTrigger

```cpp
OnceOnlyTrigger()
```

Defined in src/sched/include/icy/sched/trigger.h:116

Constructs the trigger with type "OnceOnlyTrigger".

---

{#update-4}

#### update

`virtual` `inline` `override`

```cpp
virtual inline void update() override
```

Defined in src/sched/include/icy/sched/trigger.h:119

No-op; scheduleAt is set once at construction and never advanced.

##### Reimplements

- [`update`](#update-6)

---

{#expired-3}

#### expired

`virtual` `override`

```cpp
virtual bool expired() override
```

Defined in src/sched/include/icy/sched/trigger.h:126

Returns true after the task has run at least once.

##### Reimplements

- [`expired`](#expired-4)

{#trigger-1}

## Trigger

```cpp
#include <icy/sched/trigger.h>
```

```cpp
struct Trigger
```

Defined in src/sched/include/icy/sched/trigger.h:58

> **Inherits:** [`ISerializable`](json.md#iserializable)
> **Subclassed by:** [`DailyTrigger`](#dailytrigger), [`IntervalTrigger`](#intervaltrigger), [`OnceOnlyTrigger`](#onceonlytrigger)

Base class for scheduled task triggers that determine when a task should run.

### List of all members

| Name | Kind | Owner |
|------|------|-------|
| [`type`](#type-17) | `variable` | Declared here |
| [`name`](#name-10) | `variable` | Declared here |
| [`timesRun`](#timesrun) | `variable` | Declared here |
| [`createdAt`](#createdat) | `variable` | Declared here |
| [`scheduleAt`](#scheduleat) | `variable` | Declared here |
| [`lastRunAt`](#lastrunat) | `variable` | Declared here |
| [`Trigger`](#trigger-2) | `function` | Declared here |
| [`update`](#update-6) | `function` | Declared here |
| [`remaining`](#remaining-1) | `function` | Declared here |
| [`timeout`](#timeout-4) | `function` | Declared here |
| [`expired`](#expired-4) | `function` | Declared here |
| [`serialize`](#serialize-5) | `function` | Declared here |
| [`deserialize`](#deserialize-5) | `function` | Declared here |
| [`serialize`](json.md#serialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |
| [`deserialize`](json.md#deserialize-1) | `function` | Inherited from [`ISerializable`](json.md#iserializable) |

### Inherited from [`ISerializable`](json.md#iserializable)

| Kind | Name | Description |
|------|------|-------------|
| `function` | [`serialize`](json.md#serialize-1) `virtual` | Serializes this object's state into `root`. |
| `function` | [`deserialize`](json.md#deserialize-1) `virtual` | Populates this object's state from `root`. |

### Public Attributes

| Return | Name | Description |
|--------|------|-------------|
| `std::string` | [`type`](#type-17)  | The type of this trigger class. |
| `std::string` | [`name`](#name-10)  | The display name of this trigger class. |
| `int` | [`timesRun`](#timesrun)  | The number of times the task has run since creation; |
| `DateTime` | [`createdAt`](#createdat)  | The time the task was created. |
| `DateTime` | [`scheduleAt`](#scheduleat)  | The time the task is scheduled to run. |
| `DateTime` | [`lastRunAt`](#lastrunat)  | The time the task was last run. |

---

{#type-17}

#### type

```cpp
std::string type
```

Defined in src/sched/include/icy/sched/trigger.h:90

The type of this trigger class.

---

{#name-10}

#### name

```cpp
std::string name
```

Defined in src/sched/include/icy/sched/trigger.h:93

The display name of this trigger class.

---

{#timesrun}

#### timesRun

```cpp
int timesRun
```

Defined in src/sched/include/icy/sched/trigger.h:97

The number of times the task has run since creation;

---

{#createdat}

#### createdAt

```cpp
DateTime createdAt
```

Defined in src/sched/include/icy/sched/trigger.h:100

The time the task was created.

---

{#scheduleat}

#### scheduleAt

```cpp
DateTime scheduleAt
```

Defined in src/sched/include/icy/sched/trigger.h:103

The time the task is scheduled to run.

---

{#lastrunat}

#### lastRunAt

```cpp
DateTime lastRunAt
```

Defined in src/sched/include/icy/sched/trigger.h:106

The time the task was last run.

### Public Methods

| Return | Name | Description |
|--------|------|-------------|
|  | [`Trigger`](#trigger-2)  |  |
| `void` | [`update`](#update-6) `virtual` | Updates the scheduleAt value to the next scheduled time. |
| `std::int64_t` | [`remaining`](#remaining-1) `virtual` | Returns the milliseconds remaining until the next scheduled timeout. |
| `bool` | [`timeout`](#timeout-4) `virtual` | Returns true if the task is ready to be run, false otherwise. |
| `bool` | [`expired`](#expired-4) `virtual` | Returns true if the task is expired and should be destroyed. Returns false by default. |
| `void` | [`serialize`](#serialize-5) `virtual` `override` | Serializes timing state (type, name, createdAt, scheduleAt, lastRunAt, timesRun) to `root`. |
| `void` | [`deserialize`](#deserialize-5) `virtual` `override` | Deserializes timing state from `root`. |

---

{#trigger-2}

#### Trigger

```cpp
Trigger(const std::string & type = "", const std::string & name = "")
```

Defined in src/sched/include/icy/sched/trigger.h:62

#### Parameters
* `type` Registered type name used by [TaskFactory](#taskfactory). 

* `name` Human-readable display name.

---

{#update-6}

#### update

`virtual`

```cpp
virtual void update()
```

Defined in src/sched/include/icy/sched/trigger.h:66

Updates the scheduleAt value to the next scheduled time.

##### Reimplemented by

- [`update`](#update-2)
- [`update`](#update-3)
- [`update`](#update-4)

---

{#remaining-1}

#### remaining

`virtual`

```cpp
virtual std::int64_t remaining()
```

Defined in src/sched/include/icy/sched/trigger.h:70

Returns the milliseconds remaining until the next scheduled timeout.

---

{#timeout-4}

#### timeout

`virtual`

```cpp
virtual bool timeout()
```

Defined in src/sched/include/icy/sched/trigger.h:74

Returns true if the task is ready to be run, false otherwise.

---

{#expired-4}

#### expired

`virtual`

```cpp
virtual bool expired()
```

Defined in src/sched/include/icy/sched/trigger.h:79

Returns true if the task is expired and should be destroyed. Returns false by default.

##### Reimplemented by

- [`expired`](#expired-2)
- [`expired`](#expired-3)

---

{#serialize-5}

#### serialize

`virtual` `override`

```cpp
virtual void serialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/trigger.h:83

Serializes timing state (type, name, createdAt, scheduleAt, lastRunAt, timesRun) to `root`. 
#### Parameters
* `root` JSON object to populate.

##### Reimplements

- [`serialize`](json.md#serialize-1)

##### Reimplemented by

- [`serialize`](#serialize-2)

---

{#deserialize-5}

#### deserialize

`virtual` `override`

```cpp
virtual void deserialize(json::Value & root) override
```

Defined in src/sched/include/icy/sched/trigger.h:87

Deserializes timing state from `root`. 
#### Parameters
* `root` JSON object previously produced by [serialize()](#serialize-5).

##### Reimplements

- [`deserialize`](json.md#deserialize-1)

##### Reimplemented by

- [`deserialize`](#deserialize-2)

