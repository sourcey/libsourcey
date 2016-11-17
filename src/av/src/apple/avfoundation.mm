// support GCC compiler
#ifndef __has_feature
#define __has_feature(x) 0
#endif

#include "scy/logger.h"
#include "scy/av/devicemanager.h"

#import <assert.h>
#ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
  #import <AVFoundation/AVFoundation.h>
#endif
#endif
#import <QTKit/QTKit.h>


namespace scy {
namespace av {
namespace avfoundation {


bool GetQTKitVideoDevices(Device::Type type, std::vector<Device>* devices) {
#if !__has_feature(objc_arc)
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#else
  @autoreleasepool
#endif
  {
    // NSArray* qt_capture_devices =
    //     [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];
    NSArray* qt_capture_devices;
    if (type == Device::VideoInput) {
        qt_capture_devices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];
    }
    else if(type == Device::AudioInput) {
        qt_capture_devices = [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeSound];
    }
    else {
        return false;
    }

    NSUInteger count = [qt_capture_devices count];
    DebugL << count << " capture device(s) found:" << std::endl;
    for (QTCaptureDevice* qt_capture_device in qt_capture_devices) {
      static NSString* const kFormat = @"localizedDisplayName: \"%@\", "
          @"modelUniqueID: \"%@\", uniqueID \"%@\", isConnected: %d, "
          @"isOpen: %d, isInUseByAnotherApplication: %d";
      NSString* info = [NSString
          stringWithFormat:kFormat,
                           [qt_capture_device localizedDisplayName],
                           [qt_capture_device modelUniqueID],
                           [qt_capture_device uniqueID],
                           [qt_capture_device isConnected],
                           [qt_capture_device isOpen],
                           [qt_capture_device isInUseByAnotherApplication]];
      DebugL << '\t' << [info UTF8String] << std::endl;

      std::string uniqueID([[qt_capture_device uniqueID] UTF8String]);
      std::string name([[qt_capture_device localizedDisplayName] UTF8String]);
      devices->push_back(Device(type, uniqueID, name));
    }
  }
#if !__has_feature(objc_arc)
  [pool drain];
#endif
  return true;
}


bool GetAVFoundationVideoDevices(Device::Type type, std::vector<Device>* devices) {
#ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#if __MAC_OS_X_VERSION_MAX_ALLOWED >=1070
  if (![AVCaptureDevice class]) {
    // Fallback to using QTKit if AVFoundation is not available
    return GetQTKitVideoDevices(type, devices);
  }
#if !__has_feature(objc_arc)
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#else
  @autoreleasepool
#endif
  {
    NSArray* capture_devices = [AVCaptureDevice devices];
    DebugL << [capture_devices count] << " capture device(s) found:" << std::endl;
    for (AVCaptureDevice* capture_device in capture_devices) {
      if ((type == Device::VideoInput && [capture_device hasMediaType:AVMediaTypeVideo]) ||
          (type == Device::AudioInput && [capture_device hasMediaType:AVMediaTypeAudio])) {
      // if ([capture_device hasMediaType:AVMediaTypeVideo] ||
      //     [capture_device hasMediaType:AVMediaTypeMuxed]) {
        static NSString* const kFormat = @"localizedName: \"%@\", "
            @"modelID: \"%@\", uniqueID \"%@\", isConnected: %d, "
            @"isInUseByAnotherApplication: %d";
        NSString* info = [NSString
            stringWithFormat:kFormat,
                             [capture_device localizedName],
                             [capture_device modelID],
                             [capture_device uniqueID],
                             [capture_device isConnected],
                             [capture_device isInUseByAnotherApplication]];
        DebugL << '\t' << [info UTF8String] << std::endl;

        std::string uniqueID([[capture_device uniqueID] UTF8String]);
        std::string name([[capture_device localizedName] UTF8String]);
        devices->push_back(Device(type, uniqueID, name));
      }
    }
  }
#if !__has_feature(objc_arc)
  [pool drain];
#endif
  return true;
#else  // __MAC_OS_X_VERSION_MAX_ALLOWED >=1070
  return GetQTKitVideoDevices(type, devices);
#endif  // __MAC_OS_X_VERSION_MAX_ALLOWED >=1070
#else  // __MAC_OS_X_VERSION_MAX_ALLOWED
  return GetQTKitVideoDevices(type, devices);
#endif  // __MAC_OS_X_VERSION_MAX_ALLOWED
}


} // namespace avfoundation
} // namespace av
} // namespace scy


/*
 * libjingle
 * Copyright 2010, Google Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *  3. The name of the author may not be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
