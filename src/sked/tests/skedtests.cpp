#include "scy/base.h"
#include "scy/logger.h"
//#include "scy/runner.h"
#include "scy/application.h"
#include "scy/datetime.h"
#include "scy/platform.h"
#include "scy/sked/scheduler.h"
#include <assert.h>


using namespace std;
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
	

Application app;
	

class Tests
{
	Scheduler scheduler;

public:
	Tests()
	{	
		// Register tasks and triggers
		scheduler.factory().registerTask<ScheduledTask>("ScheduledTask");
		scheduler.factory().registerTrigger<OnceOnlyTrigger>("OnceOnlyTrigger");
		scheduler.factory().registerTrigger<DailyTrigger>("DailyTrigger");
		scheduler.factory().registerTrigger<IntervalTrigger>("IntervalTrigger");

		runOnceOnlyTest();
		//runSkedTaskIntervalTest();
		//runSkedTaskTest();
		//runTimerTest();
				
		app.finalize();
	}
	

	// ---------------------------------------------------------------------
	//
	// Scheduled Task Tests
	//
	// ---------------------------------------------------------------------	
	struct ScheduledTask: public sked::Task
	{
		ScheduledTask() : 
			sked::Task("ScheduledTask")
		{
			DebugL << "Creating ################################" << endl;				
		}

		virtual ~ScheduledTask()
		{
			DebugL << "Destroying ################################" << endl;	

			// Dereference the main application 
			// loop to move on to the next test.
			app.stop();		
		}

		void run() 
		{
			DebugL << "Running ################################" << endl;
		}
		
		void serialize(json::Value& root)
		{
			sked::Task::serialize(root);

			root["RequiredField"] = "blah";
		}

		void deserialize(json::Value& root)
		{
			json::assertMember(root, "RequiredField");

			sked::Task::deserialize(root);
		}
	};

	void runOnceOnlyTest() 
	{
		json::Value json;

		// Schedule a once only task to run in 5 seconds time.
		{
			ScheduledTask* task = new ScheduledTask(); //scheduler
			OnceOnlyTrigger* trigger = task->createTrigger<OnceOnlyTrigger>();
			
			DateTime dt;
			Timespan ts(2, 0);
			dt += ts;
			trigger->scheduleAt = dt;
			assert(ts.seconds() == 2);
			
			scheduler.start(task);
			
			// Serialize the task
			scheduler.serialize(json);

			// Wait for the task to complete
			app.run();
		}	
			
		// Deserialize the previous task from JSON and run it again
		{	
			// Set the task to run in 1 secs time
			{				
				DateTime dt;
				Timespan ts(1, 0);
				dt += ts;
				json[(int)0]["trigger"]["scheduleAt"] = 
					DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT);
			}

			// Dynamically create the task from JSON
			DebugL << "Sked Input JSON:\n" 
				<< json::stringify(json, true) << endl;
			scheduler.deserialize(json);

			// Print to cout
			DebugL << "##### Sked Print Output:" << endl;
			scheduler.print(cout);
			DebugL << "##### Sked Print Output END" << endl;
			
			// Output scheduler tasks as JSON before run
			json::Value before;
			scheduler.serialize(before);
			DebugL << "Sked Output JSON Before Run:\n" 
				<< json::stringify(before, true) << endl;
			
			// Wait for the task to complete
			app.run();
			
			// Output scheduler tasks as JSON after run
			json::Value after;
			scheduler.serialize(after);
			DebugL << "Sked Output JSON After Run:\n" 
				<< json::stringify(after, true) << endl;
		}
	}

	void runSkedTaskIntervalTest() 
	{
		DebugL << "Running Scheduled Task Test" << endl;

		// Schedule an interval task to run 3 times at 1 second intervals
		{
			ScheduledTask* task = new ScheduledTask();			
			IntervalTrigger* trigger = task->createTrigger<IntervalTrigger>();
			
			trigger->interval = Timespan(1, 0);
			trigger->maxTimes = 3;

			scheduler.start(task);
			//task->start();

			// Print to cout
			DebugL << "##### Sked Print Output:" << endl;
			scheduler.print(cout);
			DebugL << "##### Sked Print Output END" << endl;
			
			// Wait for the task to complete
			app.run();
		}

		
		/*

			//ready.wait();
			//ready.wait();
			//ready.wait();

		// Schedule to fire once now, and in two days time.	
		{
			ScheduledTask* task = new ScheduledTask();			
			DailyTrigger* trigger = task->createTrigger<DailyTrigger>();

			// 2 secs from now
			DateTime dt;
			Timespan ts(2, 0);
			dt += ts;
			trigger->timeOfDay = dt;

			// skip tomorrow
			trigger->daysExcluded.push_back((DaysOfTheWeek)(dt.dayOfWeek() + 1));
			
			scheduler.schedule(task);

			// TODO: Assert running date

			ready.wait();
		}
		*/
				
		app.run();
		DebugL << "Running Scheduled Task Test: END" << endl;
	}
};


} } // namespace scy::sked


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("debug", LTrace));
	{
		sked::Tests app;
	}	
	Logger::destroy();
	//util::pause();
	return 0;
}
