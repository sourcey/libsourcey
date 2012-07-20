#include "Sourcey/PackageManager/PackageTask.h"
#include "Sourcey/PackageManager/PackageManager.h"

#include "Poco/format.h"

/*
#include "Sourcey/Base/Util.h"
#include "Sourcey/Base/Logger.h"
#include "Sourcey/Base/TaskRunner.h"

#include "Poco/Path.h"
#include "Poco/File.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/StreamCopier.h"
#include "Poco/DirectoryIterator.h"
#include "Poco/Delegate.h"

#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/FilePartSource.h"
#include "Poco/Net/NetException.h"

#include "Poco/Zip/Decompress.h"
#include "Poco/Zip/ZipArchive.h"
*/


using namespace std;
using namespace Poco;
//using namespace Poco::Net;


namespace Sourcey { 
namespace Anionu {


PackageTask::PackageTask(PackageManager& manager) :
	ITask(manager.runner(), false, false),
	Stateful<PackageTaskState>(this),
	_manager(manager)
{
}


PackageTask::~PackageTask()
{
	Util::ClearVector(_packages);
}


void PackageTask::run()
{
	if (state().id() == PackageTaskState::None) {
		setState(PackageTaskState::QueryingPackages);
	}
}


bool PackageTask::setState(unsigned int id, const string& message) 
{
	Log("trace") << "[PackageTask] Setting state: " << id << endl; 

	assert(id > state().id());

	if (Stateful<PackageTaskState>::setState(id, message)) {

		switch (id) {	
		/*
		case PackageTaskState::QueryingPackages:
			queryServerForUpdates();
			break;

		case PackageTaskState::QueryingPackagesComplete:
			for (PackageInstallerList::const_iterator it = _packages.begin(); it != _packages.end(); ++it) {	
				if (!(*it)->stateEquals(PackageTaskState::Failed) && 
					!(*it)->stateEquals(PackageTaskState::Complete))
					(*it)->doDownload();
			}
			break;
			*/

		case PackageTaskState::DownloadComplete:
			for (PackageInstallerList::const_iterator it = _packages.begin(); it != _packages.end(); ++it) {	
				if (!(*it)->stateEquals(PackageTaskState::Failed) && 
					!(*it)->stateEquals(PackageTaskState::Complete))
					(*it)->doExtract();
			}
			break;

		case PackageTaskState::ExtractionComplete:
			for (PackageInstallerList::const_iterator it = _packages.begin(); it != _packages.end(); ++it) {	
				if (!(*it)->stateEquals(PackageTaskState::Failed) && 
					!(*it)->stateEquals(PackageTaskState::Complete))
					(*it)->doFinalize();
			}
			break;			

		case PackageTaskState::FinalizingComplete:
			for (PackageInstallerList::const_iterator it = _packages.begin(); it != _packages.end(); ++it) {	
				if (!(*it)->stateEquals(PackageTaskState::Failed) && 
					!(*it)->stateEquals(PackageTaskState::Complete))
					(*it)->setComplete(format("%s successfully updated to version %s", 
						(*it)->name(), 
						string((*it)->remote.latestUpdate().attribute("version").value())));
			}
			break;

		case PackageTaskState::Complete:
		case PackageTaskState::Failed:

			// Kill the task...
			abort();
			break;
		}

		return true;
	}
	return false;
}


void PackageTask::setFailed(const string& message) 
{ 
	Log("error") << "[PackageTask] Setting Failed: " << message << endl;
	setState(PackageTaskState::Failed, message);
}


void PackageTask::setComplete(const string& message)
{
	Log("debug") << "[PackageTask] Setting Complete: " << state().message() << endl;
	setState(PackageTaskState::Complete, message);
}


void PackageTask::onPackageTaskStateChanged(void*, PackageTaskState& state, const PackageTaskState&) 
{
	Log("trace") << "[PackageTask] PackageInstaller State Changed: " << state.toString() << endl;
	
	// Once all package states are matching or failed the internal state can increment.
	for (PackageInstallerList::const_iterator it = _packages.begin(); it != _packages.end(); ++it) {	
		if (state.id() != PackageTaskState::Failed &&
			state.id() <= this->state().id()) {
			Log("trace") << "[PackageTask] Unable to transition state from " 
				<< this->state().toString() << " to " << state.toString() << endl;
			return;
		}
	}

	setState(state.id());
}


void PackageTask::addPackageInstaller(PackageInstaller* package)
{
	if (!package->valid())
		throw Exception("The package is invalid");
	_packages.push_back(package);
	package->StateChanged += delegate(this, &PackageTask::onPackageTaskStateChanged);
}


} } // namespace Sourcey::Anionu