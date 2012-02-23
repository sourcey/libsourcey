#include "Sourcey/Spot/ISynchronizer.h"
#include "Sourcey/CryptoProvider.h"

#include "Poco/Path.h"

#include <sstream>


using namespace std; 
using namespace Poco;


namespace Sourcey {
namespace Spot {


// ---------------------------------------------------------------------
//
// Suncronization Queue Job
//
// ---------------------------------------------------------------------
Job::Job() :
	id(CryptoProvider::generateRandomKey(16)), 
	priority(0), 
	state("Ready"), 
	time(Poco::DateTimeFormatter::format(Poco::Timestamp(), "%Y-%m-%d %H:%M:%S"))
{
}

	
/*
		 const std::string& state,
		 const std::string& message,
Job::Job(int priority,
		 const std::string& type,
		 const std::string& file,
		 const std::string& state,
		 const std::string& message,
		 const std::string& time) :
	type(type), 
	file(file), 
	state(state), 
	message(message), 
	time(time)
{
}
*/


Job::Job(const std::string& type,
		 const std::string& path,
		 int priority,		 
		 const std::string& time) :
	id(CryptoProvider::generateRandomKey(16)), 
	type(type), 
	path(path), 
	priority(priority), 
	state("Ready"), 
	time(time)
{
}


Job::Job(const Job& r) : 
	id(r.id), 
	priority(r.priority), 
	type(r.type), 
	path(r.path), 
	state(r.state), 
	time(r.time)
{
}


Job* Job::clone() const 
{
	return new Job(*this);
}


bool Job::valid() const
{
	return !path.empty() 
		&& (type == "Video"
		||  type == "Audio"
		||  type == "Image"
		||  type == "Archive"
		||  type == "Text");
}


string Job::toString() const
{
	ostringstream ost;
	ost << "Job["
		<< id << ":"
		<< type << ":"
		<< path << ":"
		<< state << ":"
		<< priority << ":"
		<< time << "]";
	return ost.str();
}


// ---------------------------------------------------------------------
//
// Suncronization Queue
//
// ---------------------------------------------------------------------
ISynchronizer::ISynchronizer(/*IEnvironment& env*/) //:
	//IModule(env)
{	
}
	
/*
ISynchronizer::~ISynchronizer() 
{
	Log("debug") << "[ISynchronizer] ~ISynchronizer()" << endl;
}


void ISynchronizer::start()
{
	Log("debug") << "[ISynchronizer] Starting" << endl;
	//database.initialize();
	//database.parseAndEnqueueJobs();
	_runner.start(this);
}


void ISynchronizer::stop()
{
	Log("debug") << "[ISynchronizer] Stopping" << endl;
	//_runner.stop(this);
	//Mutex::ScopedLock lock(_mutex);
	//for (JobQueue::iterator it = _activeQueue.begin(); it != _activeQueue.end(); ++it) {
	//}	
	reinterpret_cast<Task*>(this)->destroy();
	Log("debug") << "[ISynchronizer] Stopping: OK" << endl;
}


int ISynchronizer::push(const Job& job)
{
	Log("debug") << "[ISynchronizer] Pushing: " << job.toString() << endl;
	Mutex::ScopedLock lock(_mutex);

	_queue.push_back(job);
	Job& j = _queue.back();
	j.id = database.add(job);
	return j.id;
}


void ISynchronizer::run()
{
}


ISynchronizer& ISynchronizer::operator >> (const Job& job) 
{
	push(job);
	return *this;
}
*/



} } // namespace Sourcey::Spot