//
// LibSourcey
// Copyright (C) 2005, Sourcey <http://sourcey.com>
//
// LibSourcey is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// LibSourcey is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//


#ifndef SOURCEY_MEDIA_CaptureRecorder_H
#define SOURCEY_MEDIA_CaptureRecorder_H


#include "Sourcey/Media/Types.h"
#include "Sourcey/Media/VideoCapture.h"
#include "Sourcey/Media/AudioCapture.h"
#include "Sourcey/Media/CaptureEncoder.h"
#include "Sourcey/Media/AVEncoder.h"
#include "Sourcey/Stateful.h"
#include "Sourcey/PacketEmitter.h"
#include "Sourcey/Signal.h"

#include "Poco/DateTimeFormatter.h"
#include "Poco/Timestamp.h"

#include "assert.h"


namespace Sourcey {
namespace Media {


class CaptureRecorder: public AVEncoder
	/// A multiplexed recorder for AV device capture.
	///
	/// NOTE: Depreciated in favor of PacketStream API.
{
public:
	CaptureRecorder(
		const RecorderOptions& params = RecorderOptions(), 
		Media::VideoCapture* video = NULL, 
		Media::AudioCapture* audio = NULL, 
		bool destroyOnStop = false);
	virtual ~CaptureRecorder();

	virtual void initialize();
	virtual void uninitialize();

	virtual void start();
	virtual void stop();
	virtual void extend(time_t stopAt);
	virtual void cleanup();

	virtual void setVideoCapture(Media::VideoCapture* video);
	virtual void setAudioCapture(Media::AudioCapture* audio);
		// NOTE: Captures must be set before the encoder is 
		// initialized.

	virtual void onVideoCapture(void*, VideoPacket& packet);
	virtual void onAudioCapture(void*, AudioPacket& packet);

private:
	Poco::FastMutex		_mutex;
	VideoCapture*		_video;
	AudioCapture*		_audio;
	bool				_destroyOnStop;
};


} } // namespace Sourcey::Media


#endif // SOURCEY_MEDIA_CaptureRecorder_H