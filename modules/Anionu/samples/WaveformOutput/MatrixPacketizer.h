#ifndef SOURCEY_MEDIA_MatrixPacketizer_H
#define SOURCEY_MEDIA_MatrixPacketizer_H


#include "Sourcey/Base/IPacketizer.h"
#include "Sourcey/Base/Signal.h"

extern "C" {
#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>
//#include <libavutil/fifo.h>
#include <libswscale/swscale.h>
}

namespace Sourcey { 
namespace Media {


class MatrixPacketizer: public IPacketizer
{
public:
	MatrixPacketizer() : //const std::string& contentType
		convCtx(NULL)
	{
	}
	
	//virtual void writeCustomMEDIAHeaders(std::ostringstream&)
		// This method may be overridden to add custom Media headers.
	//{	
	//}
	
	/*
	virtual int populate()
	{
	}
	*/
	virtual bool accepts(IPacket& packet) 
	{ 
		return dynamic_cast<const Media::VideoPacket*>(&packet) != 0; 
	}
	
	/*
	virtual int populate()
	{
	}

	int populateMatrix(cv::Mat& mat)
	{
		if (!video || !video->error.empty()) {
			Log("warning") << "[AVFileReader:" << this << "] Decoding Video: Not initialized." << endl;
			return -1;
		}
	}
	*/

	virtual void process(IPacket& packet)
	{
		VideoPacket& vpacket = reinterpret_cast<VideoPacket&>(packet);
		VideoDecoderContext* video = reinterpret_cast<VideoDecoderContext*>(packet.opaque);	
		if (video == NULL)
			throw Exception("Video packets must contain a VideoDecoderContext.");		
	
		// Convert the image from its native format to BGR.
		if (convCtx == NULL) {
			convCtx = sws_getContext(
				video->codec->width, video->codec->height, video->codec->pix_fmt, 
				video->codec->width, video->codec->height, PIX_FMT_BGR24, 
				SWS_BICUBIC, NULL, NULL, NULL);
			mat.create(video->codec->height, video->codec->width, CV_8UC(3));
		}
		if (convCtx == NULL)
			throw Exception("Matrix Packetizer: Unable to initialize the conversion context.");	
			
		// Scales the data in src according to our settings in our SwsContext.
		if (sws_scale(convCtx,
			video->iframe->data, video->iframe->linesize, 0, video->codec->height,
			video->oframe->data, video->oframe->linesize) < 0)
			throw Exception("Matrix Packetizer: Pixel format conversion not supported.");

		// Populate the OpenCV Matrix.
		for (int y = 0; y < video->codec->height; y++) {
			for (int x = 0; x < video->codec->width; x++) {
				mat.at<cv::Vec3b>(y,x)[0]=video->oframe->data[0][y * video->oframe->linesize[0] + x * 3 + 0];
				mat.at<cv::Vec3b>(y,x)[1]=video->oframe->data[0][y * video->oframe->linesize[0] + x * 3 + 1];
				mat.at<cv::Vec3b>(y,x)[2]=video->oframe->data[0][y * video->oframe->linesize[0] + x * 3 + 2];
			}
		}

		vpacket.mat = &mat;
		broadcast(vpacket);

		/*
		Buffer buf;
		packet.write(buf);
		
		int contentLength = 0;
		int headerLength = 0;
		std::ostringstream header;

		// Send the initial Media response header		
		if (packetNumber() == 0) {	
			header << "MEDIA/1.1 200 OK\r\n"
				   << "Content-Type: multipart/x-mixed-replace; boundary=end\r\n"
				   << "Cache-Control: no-store\r\n"
				   << "Pragma: no-cache\r\n"
				   << "Expires: 0\r\n"
				   << "\r\n";
		}

		header << "--end\r\n"
				<< "Content-Type: " << contentType << "\r\n";

		// Write custom headers
		writeCustomMEDIAHeaders(header);
		header << "\r\n";
		
		headerLength = header.str().size();
		contentLength = buf.length(); //std::min<int>((int)buf.length(), (int)_mtu - headerLength);	
		
		Log("trace") << "[MatrixPacketizer" << this << "] Current Packet\n" 
			<< "\n\tHeader Length: " << headerLength
			<< "\n\tContent Length: " << contentLength
			<< "\n\tTotal Length: " << headerLength + contentLength
			//<< "\n\tBytes Remaining: " << buf.remaining()
			<< std::endl;
		
		Buffer obuf;
		obuf.writeString(header.str());
		obuf.writeBytes(buf.data(), contentLength);

		DataPacket opacket;
		opacket.read(obuf);
			
		// Broadcast the packet
		broadcast(opacket);
		*/
	}

	cv::Mat mat;
	struct SwsContext* convCtx;

	//std::string contentType;
};


} } // namespace Sourcey::MEDIA


#endif
