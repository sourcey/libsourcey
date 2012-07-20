#include "Sourcey/Media/MediaProvider.h"


using namespace std;
using namespace Poco;


namespace Sourcey {
namespace Media {


// ============================================================================
//
// Media Provider
//
// ============================================================================
MediaProvider::MediaProvider(bool destroyOnStop) : 
	_destroyOnStop(destroyOnStop) {
	cout << "MediaProvider::MediaProvider()" << endl;
}


MediaProvider::~MediaProvider() {
	cout << "MediaProvider::~MediaProvider()" << endl;
}


} } // namespace Sourcey::Media