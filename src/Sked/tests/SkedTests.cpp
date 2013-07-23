#include "Sourcey/Base.h"
#include "Sourcey/Logger.h"
#include "Sourcey/Runner.h"
#include "Sourcey/Application.h"
#include "Sourcey/DateTime.h"
#include "Sourcey/Platform.h"
#include "Sourcey/Sked/Scheduler.h"

//#include "Poco/NamedEvent.h"

#include <assert.h>


using namespace std;
//using namespace Poco;
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
	

//static NamedEvent ready("TestEvent");
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
				
		app.cleanup();
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
			debugL("ScheduledTask") << "Creating ################################" << endl;				
		}

		virtual ~ScheduledTask()
		{
			debugL("ScheduledTask") << "Destroying ################################" << endl;	

			// Dereference the main application 
			// loop to move on to the next test.
			app.stop();		
		}

		void run() 
		{
			debugL("ScheduledTask") << "Running ################################" << endl;
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
				json[(size_t)0]["trigger"]["scheduleAt"] = 
					DateTimeFormatter::format(dt, DateTimeFormat::ISO8601_FORMAT);
			}

			// Dynamically create the task from JSON
			Log("debug") << "Sked Input JSON:\n" 
				<< json::stringify(json, true) << endl;
			scheduler.deserialize(json);

			// Print to cout
			Log("debug") << "##### Sked Print Output:" << endl;
			scheduler.print(cout);
			Log("debug") << "##### Sked Print Output END" << endl;
			
			// Output scheduler tasks as JSON before run
			json::Value before;
			scheduler.serialize(before);
			Log("debug") << "Sked Output JSON Before Run:\n" 
				<< json::stringify(before, true) << endl;
			
			// Wait for the task to complete
			app.run();
			
			// Output scheduler tasks as JSON after run
			json::Value after;
			scheduler.serialize(after);
			Log("debug") << "Sked Output JSON After Run:\n" 
				<< json::stringify(after, true) << endl;
		}
	}

	void runSkedTaskIntervalTest() 
	{
		Log("trace") << "Running Scheduled Task Test" << endl;

		// Schedule an interval task to run 3 times at 1 second intervals
		{
			ScheduledTask* task = new ScheduledTask();			
			IntervalTrigger* trigger = task->createTrigger<IntervalTrigger>();
			
			trigger->interval = Timespan(1, 0);
			trigger->maxTimes = 3;

			scheduler.start(task);
			//task->start();

			// Print to cout
			Log("debug") << "##### Sked Print Output:" << endl;
			scheduler.print(cout);
			Log("debug") << "##### Sked Print Output END" << endl;
			
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
		Log("trace") << "Running Scheduled Task Test: END" << endl;
	}
};


} } // namespace scy::sked


int main(int argc, char** argv) 
{	
	Logger::instance().add(new ConsoleChannel("Test", TraceLevel));
	{
		sked::Tests app;
	}	
	Logger::uninitialize();
	//util::pause();
	return 0;
}