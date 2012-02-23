#include "Sourcey/Spot/IChannel.h"
#include "Sourcey/Media/CaptureFactory.h"


using namespace std; 
using namespace Poco; 
using namespace Sourcey::Media; 


namespace Sourcey {
namespace Spot {

	
IChannel::IChannel(/*IEnvironment& env*//*, int id,*/ const string& name, const Media::Device& video, const Media::Device& audio) :
	//IModule(env),
	//_id(id),
	_name(name),
	_videoDevice(video),
	_audioDevice(audio)
{	
	assert(!_name.empty());
}

	
IChannel::IChannel(/*IEnvironment& env*//*, int id,*/ const string& name) :
	//IModule(env),
	//_id(id),
	_name(name)
{	
	assert(!_name.empty());
}


IChannel::~IChannel() 
{
}


/*
string IChannel::id() const   
{ 
	FastMutex::ScopedLock lock(_mutex);

	return _id; 
}
*/


string IChannel::name() const   
{ 
	FastMutex::ScopedLock lock(_mutex);

	return _name; 
}


string IChannel::videoInputFile() const   
{ 
	FastMutex::ScopedLock lock(_mutex);

	return _videoInputFile; 
}


Device IChannel::videoDevice() const 
{ 
	FastMutex::ScopedLock lock(_mutex);

	return _videoDevice; 
}


Device IChannel::audioDevice() const 
{ 
	FastMutex::ScopedLock lock(_mutex);

	return _audioDevice; 
}


bool IChannel::valid() const
{
	FastMutex::ScopedLock lock(_mutex);

	return !_name.empty() && _videoDevice.id >= 0;
}


VideoCapture* IChannel::videoCapture(bool whiny) const 
{
	FastMutex::ScopedLock lock(_mutex);

	VideoCapture* capture = !_videoInputFile.empty() ?
		CaptureFactory::instance()->video.getCapture(_videoInputFile) : 
		_videoDevice.id >= 0 ? 
			CaptureFactory::instance()->video.getCapture(_videoDevice.id) : NULL;
		
	if (whiny && capture == NULL) 
		throw NotFoundException(_name + ": No video device");

	return capture;
}


AudioCapture* IChannel::audioCapture(bool whiny) const
{
	FastMutex::ScopedLock lock(_mutex);

	AudioCapture* capture = _audioDevice.id >= 0 ? 
		CaptureFactory::instance()->audio.getCapture(_audioDevice.id) : NULL;
		
	if (whiny && capture == NULL) 
		throw NotFoundException(_name + ": No audio device");

	return capture;
}


void IChannel::print(ostream& ost) const 
{
	FastMutex::ScopedLock lock(_mutex);

	ost << "Channel["
		<< "\n\tName:" << _name
		<< "\n\tVideo:" << _videoDevice.name
		<< "\n\tAudio:" << _audioDevice.name
		<< "\n\tVideo ID:" << _videoDevice.id
		<< "\n\tAudio ID:" << _audioDevice.id
		<< "]";
}


/*
#include "Sourcey/Spot/IModeManager.h"

#include "Sourcey/Util.h"
#include "Sourcey/Logger.h"
#include "Poco/Format.h"
#include "Poco/Base64Encoder.h"
// ---------------------------------------------------------------------
//
// IChannel
//
// ---------------------------------------------------------------------
IChannel::IChannel(const string& name, const Media::Device& video, const Media::Device& audio) :
	_name(name),
	_videoDevice(video),
	_audioDevice(audio),
	_modes(NULL) 
{	
}


IChannel::IChannel(const string& name) :
	_name(name),
	_modes(NULL) 
{	
}


IChannel::~IChannel() 
{
	if (_modes)
		delete _modes;
}


bool IChannel::valid() const 
{
	return !_name.empty()
		&& _videoDevice.id >= 0;
}


void IChannel::setName(const string& name) 
{ 
	string oldName(_name);
	_name = name; 

	// NOTE: The callback name reflects the old channel name.
	// Delegates can use the name() method to match the name.
	ChannelChanged.dispatch(this, oldName);
}


void IChannel::setVideoDevice(const Media::Device& dev) 
{ 
	_videoDevice = dev; 
	ChannelChanged.dispatch(this, _name);
}


void IChannel::setAudioDevice(const Media::Device& dev) 
{ 
	_audioDevice = dev; 
	ChannelChanged.dispatch(this, _name);
}


void IChannel::setIModeManager(IModeManager* modes) 
{ 
	_modes = modes; 
}


VideoCapture* IChannel::videoCapture() const 
{
	return _videoDevice.id >= 0 ? CaptureFactory::instance()->video.getCapture(_videoDevice.id) : 0;
}


AudioCapture* IChannel::audioCapture() const
{
	return _audioDevice.id >= 0 ? CaptureFactory::instance()->audio.getCapture(_audioDevice.id) : 0;
}


VideoPacket* IChannel::getSnapshot(int width, int height, bool base64) const
{
	Logger::send("debug") << "IChannel::getSnapshot()" << endl;

	VideoCapture* capture = videoCapture(); 
	if (!capture)
		throw Exception(_name + ": No video capture is available");

	Logger::send("debug") << "IChannel::getSnapshot() 1" << endl;

	cv::Mat frame = capture->getFrame(width, height);
	int params[3] = {0};
	params[0] = CV_IMWRITE_JPEG_QUALITY;
	params[1] = 80; // default: 80% quality
	CvMat *jpeg = cvEncodeImage(".jpg", &frame.operator IplImage(), params);
	if (!jpeg->data.s)
		throw Exception(_name + ": Unable to encode JPEG image");

	Logger::send("debug") << "IChannel::getSnapshot() 2" << endl;

	VideoPacket* packet = new VideoPacket;
	packet->width = width;
	packet->height = height;

	if (base64) {
	Logger::send("debug") << "IChannel::getSnapshot() 3" << endl;
		ostringstream ostr;
		Poco::Base64Encoder encoder(ostr);
		encoder.write((const char*)jpeg->data.ptr, jpeg->width);
		encoder.close();
		string res = ostr.str();
	Logger::send("debug") << "IChannel::getSnapshot() 4" << endl;
		packet->data = new unsigned char[res.length()];
		memcpy(packet->data, res.data(), res.length());
		packet->size = res.length();
	Logger::send("debug") << "IChannel::getSnapshot() 5" << endl;
	}
	else {
		packet->data = new unsigned char[jpeg->width];
		memcpy(packet->data, jpeg->data.ptr, jpeg->width);
		packet->size = jpeg->width;
	}

	cvReleaseMat(&jpeg);

	Logger::send("debug") << "IChannel::getSnapshot() 6" << endl;
	return packet;
}
*/

/*



IChannelData IChannel::data()  const
{
	Logger::send("debug") << "IChannel::data()" << endl;
	//FastMutex::ScopedLock lock(_mutex);	
	return IChannelData(
		_name,
		_videoDevice,
		_audioDevice//,
		//(_modes && _modes->//_isInitialized) ? _modes->getIModesDataList() : IModesDataList()
	);
}

void IChannel::toXML(ostream& ost)
{	
	Logger::send("debug") << "IChannel::toXML()" << endl;
	//FastMutex::ScopedLock lock(_mutex);
	ost << "<channel"
		<< " name=\"" << Util::escape(_name) << "\">";
	if (_videoDevice.id >= 0) {
		ost << "<device"
			<< " type=\"" << _videoDevice.type << "\""
			<< " id=\"" << _videoDevice.id << "\""
			<< " name=\"" << Util::escape(_videoDevice.name) << "\""
			<< " available=\"" << (_videoDevice.guid.empty() ? "false" : "true") << "\">";
	}
	if (_audioDevice.id >= 0) {
		ost << "<device"
			<< " type=\"" << _audioDevice.type << "\""
			<< " id=\"" << _audioDevice.id << "\""
			<< " name=\"" << Util::escape(_audioDevice.name) << "\""
			<< " available=\"" << (_videoDevice.guid.empty() ? "false" : "true") << "\">";
	}
	//recorder.toXML(ost);
	//modes.toXML(ost);
	ost << "</channel>";
}


string IChannel::toString() const {
	ostringstream ss;
	ss  << "IChannel["
		<< " name:" << _name
		<< " Video:" << _videoDevice.name
		<< " Audio:" << _audioDevice.name
		<< " Video ID:" << _videoDevice.id
		<< " Audio ID:" << _audioDevice.id
		<< "]";
	return ss.str();
}

*/

// ---------------------------------------------------------------------
//
// IChannel IMode Controller
//
// ---------------------------------------------------------------------
/*
IChannel::IModes::IModes(IChannel* channel) : 
	_channel(channel)
{
	Logger::send("debug") << "IChannel::IModes::IModes(): " << this << endl;

	// Instantiate each available mode.
	IModeManager::ModeMapT* modes = IEnvironment::instance().modes().map();		
	for (IModeManager::ModeMapT::iterator it = modes->begin(); it != modes->end(); ++it) {	
		IMode* mode = IEnvironment::instance().modes().createInstance(it->first, _channel);
		if (mode) {
			_modes[it->first] = mode;
		}
	}

	IEnvironment::instance().GenerateCommandsXML += delegate(this, &IChannel::IModes::doGenerateCommandsXML);
	IEnvironment::instance().router().RecvStanza += delegate(this, &IChannel::IModes::onRecvStanza);
}


IChannel::IModes::~IModes() 
{	
	Logger::send("debug") << "IChannel::IModes::~IModes(): " << this << endl;
	clear();

	//assert(false);
	//IEnvironment::instance().GenerateCommandsXML -= delegate(this, &IChannel::IModes::doGenerateCommandsXML);
	//IEnvironment::instance().router().RecvStanza -= delegate(this, &IChannel::IModes::onRecvStanza);

	Logger::send("debug") << "IChannel::IModes::~IModes(): OK: " << this << endl;
}


void IChannel::IModes::clear()
{
	ClearMap(_modes);
}


IMode* IChannel::IModes::get(const string& name, bool whiny)
{
	//FastMutex::ScopedLock lock(_mutex);
	ModeMap::iterator it = _modes.find(name);
	if (it != _modes.end()) {
		return it->second;	 
	} else if (whiny)
		throw NotFoundException("Invalid mode requested: " + name);	
	return NULL;
}


bool IChannel::IModes::exists(const string& name)
{ 
	ModeMap::iterator it = _modes.find(name);
	return it != _modes.end();  
}


ModeMap IChannel::IModes::map()
{
	return _modes;
}


void IChannel::IModes::toXML(ostream& ost)
{	
	Logger::send("debug") << "IChannel::IModes::toXML()" << endl;
	//FastMutex::ScopedLock lock(_mutex);

	ost << "<modes>";	
	for (ModeMap::iterator it = _modes.begin(); it != _modes.end(); ++it) {	
		IMode* mode = it->second;
		ModeMap::iterator ait = _modes.find(it->first);	
		ost << "<mode "
			<< "name=\"" << Util::escape(mode->name()) << "\" "
			<< "state=\"" << mode->state() << "\">";
		for (list<StringMap>::iterator bit = mode->_data.begin(); bit != mode->_data.end(); ++bit) {	
			ost << "<var name=\"" << Util::escape(bit->first) << "\">"
				<< Util::escape(bit->second)
				<< "</var>";
		}
		ost << "</mode>";
	}
	ost << "</modes>";
}

	*/

/*
IModesDataList IChannel::IModes::getIModesDataList()
{	
	Logger::send("debug") << "IChannel::IModes::getIModesDataList()" << endl;
	IModesDataList data;
	for (ModeMap::iterator it = _modes.begin(); it != _modes.end(); ++it) {	
		data.push_back(
			IModeData(
				(*it)->_id,
				(*it)->name(),
				(*it)->_state,
				(*it)->_data
			)
		);
	}
	return data;
}
*/


/*
void IChannel::IModes::doGenerateCommandsXML(CommandList& commands)
{
	try
	{		
		for (ModeMap::iterator it = _modes.begin(); it != _modes.end(); ++it) {
			commands.add(					
				format("channels:%s:modes:%s:%s", _channel->name(), it->second->name(), it->second->state() == "active" ? string("deactivate") : string("activate")),
				format("%s %s on %s", it->second->state() == "active" ? string("Deactivate") : string("Activate"), it->second->name(), _channel->name())
			);
			commands.add(					
				format("channels:%s:modes:%s:config", _channel->name(), it->second->name()),
				format("CIConfigure %s on %s", it->second->name(), _channel->name())
			);
			commands.add(					
				format("channels:%s:modes:%s:help", _channel->name(), it->second->name()),
				format("Get help for %s on %s", it->second->name(), _channel->name())
			);
		}
	}
	catch (...)
	{
	}
}
*/


} } // namespace Sourcey::Spot