#include "Sourcey/Spot/IConfigurable.h"
#include "Sourcey/Spot/IChannel.h"
#include "Sourcey/Spot/IEnvironment.h"
#include "Sourcey/Spot/IConfiguration.h"
#include "Sourcey/Symple/Form.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Spot {


IConfigurable::IConfigurable()
{
}


IConfigurable::~IConfigurable()
{
}


bool IConfigurable::isConfigurable() const 
{
	return false;
}


bool IConfigurable::hasParsableConfig(Symple::Form& form) const
{
	return isConfigurable();
}


void IConfigurable::createConfigForm(Symple::Form& form, Symple::FormElement& element, bool useBase) 
{	
}


void IConfigurable::parseConfigForm(Symple::Form& form, Symple::FormElement& element) //, StringMap& errors
{
}


void IConfigurable::createHelp(ostream& ost)
{
	ost << "No help is available." << endl;
}



} } // namespace Sourcey::Spot