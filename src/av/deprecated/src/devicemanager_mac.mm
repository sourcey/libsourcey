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

// support GCC compiler
#ifndef __has_feature
#define __has_feature(x) 0
#endif

// #include "talk/media/devices/devicemanager.h"
#include "scy/av/devicemanager.h"

#import <assert.h>
#ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#if __MAC_OS_X_VERSION_MAX_ALLOWED >= 1070
  #import <AVFoundation/AVFoundation.h>
#endif
#endif
#import <QTKit/QTKit.h>

// #include "webrtc/base/logging.h"
#include "scy/logger.h"

@interface DeviceWatcherImpl : NSObject {
 @private
  scy::av::IDeviceManager* manager_;
}
- (id)init:(scy::av::IDeviceManager*)manager;
- (void)onDevicesChanged:(NSNotification*)notification;
@end

@implementation DeviceWatcherImpl
- (id)init:(scy::av::IDeviceManager*)manager {
  if ((self = [super init])) {
    assert(manager != NULL);
    manager_ = manager;
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onDevicesChanged:)
               name:QTCaptureDeviceWasConnectedNotification
             object:nil];
    [[NSNotificationCenter defaultCenter]
        addObserver:self
           selector:@selector(onDevicesChanged:)
               name:QTCaptureDeviceWasDisconnectedNotification
             object:nil];
  }
  return self;
}

- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
#if !__has_feature(objc_arc)
  [super dealloc];
#endif
}
- (void)onDevicesChanged:(NSNotification*)notification {
  bool isVideo = true;
  bool isConnect = true;
  manager_->DevicesChanged.emit(manager_, isVideo, isConnect); //SignalDevicesChange();
}
@end

namespace scy {
namespace av {

DeviceWatcherImpl* CreateDeviceWatcherCallback(
    IDeviceManager* manager) {
  DeviceWatcherImpl* impl;
#if !__has_feature(objc_arc)
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#else
  @autoreleasepool
#endif
  { impl = [[DeviceWatcherImpl alloc] init:manager]; }
#if !__has_feature(objc_arc)
  [pool drain];
#endif
  return impl;
}

void ReleaseDeviceWatcherCallback(DeviceWatcherImpl* watcher) {
#if !__has_feature(objc_arc)
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
  [watcher release];
  [pool drain];
#endif
}

bool GetQTKitVideoDevices(std::vector<Device>* devices) {
#if !__has_feature(objc_arc)
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#else
  @autoreleasepool
#endif
  {
    NSArray* qt_capture_devices =
        [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];
    NSUInteger count = [qt_capture_devices count];
    SInfo << count << " capture device(s) found:";
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
      SInfo << [info UTF8String];

      std::string name([[qt_capture_device localizedDisplayName] UTF8String]);
      devices->push_back(
            Device("video", devices->size() - 1, name)); //[[capture_device uniqueID] UTF8String]
    }
  }
#if !__has_feature(objc_arc)
  [pool drain];
#endif
  return true;
}

bool GetAVFoundationVideoDevices(std::vector<Device>* devices) {
#ifdef __MAC_OS_X_VERSION_MAX_ALLOWED
#if __MAC_OS_X_VERSION_MAX_ALLOWED >=1070
  if (![MediaCaptureDevice class]) {
    // Fallback to using QTKit if AVFoundation is not available
    return GetQTKitVideoDevices(devices);
  }
#if !__has_feature(objc_arc)
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];
#else
  @autoreleasepool
#endif
  {
    NSArray* capture_devices = [MediaCaptureDevice devices];
    SInfo << [capture_devices count] << " capture device(s) found:";
    for (MediaCaptureDevice* capture_device in capture_devices) {
      if ([capture_device hasMediaType:AVMediaTypeVideo] ||
          [capture_device hasMediaType:AVMediaTypeMuxed]) {
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
        SInfo << [info UTF8String];

        std::string name([[capture_device localizedName] UTF8String]);
        devices->push_back(
            Device("video", devices->size() - 1, name)); //[[capture_device uniqueID] UTF8String]
      }
    }
  }
#if !__has_feature(objc_arc)
  [pool drain];
#endif
  return true;
#else  // __MAC_OS_X_VERSION_MAX_ALLOWED >=1070
  return GetQTKitVideoDevices(devices);
#endif  // __MAC_OS_X_VERSION_MAX_ALLOWED >=1070
#else  // __MAC_OS_X_VERSION_MAX_ALLOWED
  return GetQTKitVideoDevices(devices);
#endif  // __MAC_OS_X_VERSION_MAX_ALLOWED
}

} // namespace av
} // namespace scy
