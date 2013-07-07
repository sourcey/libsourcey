#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Sked/Scheduler.h"

#include "Poco/NamedEvent.h"

#include <assert.h>


using namespace std;
using namespace Poco;
using namespace scy;


/*
// Detect Memory Leaks
#ifdef _DEBUG
#include "MemLeakDetect/MemLeakDetect.h"
CMemLeakDetect memLeakDetect;
#endif
*/


namespace scy {
namespace sked {
	

static NamedEvent ready("TestEvent");
	

class Tests
{
	//Runner runner;
	Scheduler scheduler;

public:
	Tests()
	{	
		// Register tasks and triggers
		scheduler.factory().registerTask<ScheduledTask>("ScheduledTask");
		scheduler.factory().registerTrigger<OnceOnlyTrigger>("OnceOnlyTrigger");
		scheduler.factory().registerTrigger<DailyTrigger>("DailyTrigger");
		scheduler.factory().registerTrigger<IntervalTrigger>("IntervalTrigger");

		runSkedTaskTest();
		//runTimerTest();
		
		util::pause();
	}
	

	// ---------------------------------------------------------------------
	//
	// Scheduled Task Tests
	//
	// ---------------------------------------------------------------------	
	struct ScheduledTask: public sked::Task
	{
		ScheduledTask() : //Scheduler& scheduler
			sked::Task("ScheduledTask") //scheduler, 
		{
			Log("debug") << "[ScheduledTask] Creating ################################" << endl;				
		}

		virtual ~ScheduledTask()
		{
			Log("debug") << "[ScheduledTask] Destroying ################################" << endl;			
		}

		void run() 
		{
			Log("debug") << "[ScheduledTask] Running ################################" << endl;
			ready.set();	
		}
		
		void serialize(JSON::Value& root)
		{
			sked::Task::serialize(root);

			root["RequiredField"] = "blah";
		}

		void deserialize(JSON::Value& root)
		{
			JSON::assertMember(root, "RequiredField");

			sked::Task::deserialize(root);
		}
	};

	void runSkedTaskTest() 
	{
		Log("trace") << "Running Scheduled Task Test" << endl;

		JSON::Value json;

		/*
		// Schedule a once only task to run in 2 seconds time.
		{
			ScheduledTask* task = new ScheduledTask(scheduler);
			OnceOnlyTrigger* trigger = task->createTrigger<OnceOnlyTrigger>();

			Poco::DateTime dt;
			Poco::Timespan ts(1, 0);
			dt += ts;
			trigger->scheduleAt = dt;
			
			task->start();
			
			// Serialize the task
			scheduler.serialize(json);

			// Wait for the task to run
			ready.wait();
		}	
		
		// Deserialize the previous task from JSON and run it again
		{	
			// Set the task to run in 1 secs time
			{				
				Poco::DateTime dt;
				Poco::Timespan ts(1, 0);
				dt += ts;
				json[(size_t)0]["trigger"]["scheduleAt"] = 
					DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT);
			}

			// Dynamically create the task from JSON
			Log("debug") << "Sked Input JSON:\n" 
				<< JSON::stringify(json, true) << endl;
			scheduler.deserialize(json);

			// Print to cout
			Log("debug") << "##### Sked Print Output:" << endl;
			scheduler.print(cout);
			Log("debug") << "##### Sked Print Output END" << endl;
			
			// Output scheduler tasks as JSON before run
			JSON::Value before;
			scheduler.serialize(before);
			Log("debug") << "Sked Output JSON Before Run:\n" 
				<< JSON::stringify(before, true) << endl;

			// Wait for the task to be run
			ready.wait();
			
			// Output scheduler tasks as JSON after run
			JSON::Value after;
			scheduler.serialize(after);
			Log("debug") << "Sked Output JSON After Run:\n" 
				<< JSON::stringify(after, true) << endl;
		}
				*/

		// Schedule an interval task to run 3 times at 1 second intervals
		{
			ScheduledTask* task = new ScheduledTask();			
			IntervalTrigger* trigger = task->createTrigger<IntervalTrigger>();
			
			trigger->interval = Timespan(1, 0);
			trigger->maxTimes = 3;

			scheduler.schedule(task);
			//task->start();

			// Print to cout
			Log("debug") << "##### Sked Print Output:" << endl;
			scheduler.print(cout);
			Log("debug") << "##### Sked Print Output END" << endl;

			ready.wait();
			ready.wait();
			ready.wait();
		}

		
		/*
		// Schedule to fire once now, and in two days time.	
		{
			ScheduledTask* task = new ScheduledTask();			
			DailyTrigger* trigger = task->createTrigger<DailyTrigger>();

			// 2 secs from now
			Poco::DateTime dt;
			Poco::Timespan ts(2, 0);
			dt += ts;
			trigger->timeOfDay = dt;

			// skip tomorrow
			trigger->daysExcluded.push_back((DaysOfTheWeek)(dt.dayOfWeek() + 1));
			
			scheduler.schedule(task);

			// TODO: Assert running date

			ready.wait();
		}
		*/
		
		Log("trace") << "Running Scheduled Task Test: END" << endl;
		util::pause();
	}
};


} } // namespace scy::Sked


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("Test", TraceLevel));
	{
		sked::Tests app;
	}	
	Logger::uninitialize();
	util::pause();
	return 0;
}