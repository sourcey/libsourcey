#include "scy/base.h"
#include "scy/test.h"
#include "scy/logger.h"
#include "scy/datetime.h"
#include "scy/sked/scheduler.h"


using namespace std;
using namespace scy;
using namespace scy::test;


static sked::Scheduler scheduler;
static int taskRunTimes = 0;


// =============================================================================
// Test Scheduled Task
//
struct ScheduledTask: public sked::Task
{
    ScheduledTask() :
        sked::Task("ScheduledTask")
    {
    }

    virtual ~ScheduledTask()
    {
    }

    void run()
    {
        taskRunTimes++;
    }

    void serialize(json::Value& root)
    {
        sked::Task::serialize(root);

        root["CustomField"] = "blah";
    }

    void deserialize(json::Value& root)
    {
        json::assertMember(root, "CustomField");

        sked::Task::deserialize(root);
    }
};


int main(int argc, char** argv)
{
    // Logger::instance().add(new ConsoleChannel("debug", LTrace));
    test::initialize();

    // Register tasks and triggers
    scheduler.factory().registerTask<ScheduledTask>("ScheduledTask");
    scheduler.factory().registerTrigger<sked::OnceOnlyTrigger>("OnceOnlyTrigger");
    scheduler.factory().registerTrigger<sked::DailyTrigger>("DailyTrigger");
    scheduler.factory().registerTrigger<sked::IntervalTrigger>("IntervalTrigger");

    describe("once only task serialization", []() {
        json::Value json;
        Timespan hundredMs(0, 1);

        // Schedule a once only task to run in 100ms time.
        {
            taskRunTimes = 0;

            auto task = new ScheduledTask();
            auto trigger = task->createTrigger<sked::OnceOnlyTrigger>();

            DateTime dt;
            dt += hundredMs;
            trigger->scheduleAt = dt;

            scheduler.start(task);

            // Serialize the task
            scheduler.serialize(json);

            // Wait for the task to complete
            scy::sleep(1500);
            expect(taskRunTimes == 1);
        }

        // Deserialize the previous task from JSON and run it again
        {
            taskRunTimes = 0;

            // Set the task to run in 100ms time
            {
                DateTime dt;
                dt += hundredMs;
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
            scy::sleep(1500);
            expect(taskRunTimes == 1);

            // Output scheduler tasks as JSON after run
            json::Value after;
            scheduler.serialize(after);
            DebugL << "Sked Output JSON After Run:\n"
                << json::stringify(after, true) << endl;
        }
    });

    describe("interval task", []() {
        DebugL << "Running Scheduled Task Test" << endl;

        taskRunTimes = 0;

        // Schedule an interval task to run 3 times at 100ms intervals
        {
            auto task = new ScheduledTask();
            sked::IntervalTrigger* trigger = task->createTrigger<sked::IntervalTrigger>();

            trigger->interval = Timespan(0, 1);
            trigger->maxTimes = 3;

            scheduler.start(task);

            // Print to cout
            DebugL << "##### Sked Print Output:" << endl;
            scheduler.print(cout);
            DebugL << "##### Sked Print Output END" << endl;

            // Wait for the task to complete
            scy::sleep(1000);
            expect(taskRunTimes == 3);
        }

        DebugL << "Running Scheduled Task Test: END" << endl;
    });

    // // Schedule to fire once now, and in two days time.
    // {
    //     auto task = new ScheduledTask();
    //     auto trigger = task->createTrigger<DailyTrigger>();
    //
    //     // 2 secs from now
    //     DateTime dt;
    //     Timespan ts(2, 0);
    //     dt += ts;
    //     trigger->timeOfDay = dt;
    //
    //     // skip tomorrow
    //     trigger->daysExcluded.push_back((DaysOfTheWeek)(dt.dayOfWeek() + 1));
    //
    //     scheduler.schedule(task);
    //
    //     // TODO: Assert running date
    //     expect(task->trigger().remaining() == 1);
    //     expect(task->trigger().timesRun == 1);
    // }

    test::runAll();

    Logger::destroy();

    return test::finalize();
}
