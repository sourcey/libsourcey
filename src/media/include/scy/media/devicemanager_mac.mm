/*
 * libjingle
 * Copyright 2004--2010, Google Inc.
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

#include "talk/session/phone/devicemanager.h"

#import <QTKit/QTKit.h>

#include "talk/base/logging.h"

@interface DeviceWatcherImpl : NSObject {
@private
  cricket::DeviceManager* manager_;
}
- (id)init:(cricket::DeviceManager*) dm;
- (void)onDevicesChanged:(NSNotification *)notification;
@end

@implementation DeviceWatcherImpl
- (id)init:(cricket::DeviceManager*) dm {
  if ((self = [super init])) {
    manager_ = dm;
    [[NSNotificationCenter defaultCenter] addObserver:self
        selector:@selector(onDevicesChanged:)
        name:QTCaptureDeviceWasConnectedNotification
        object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
        selector:@selector(onDevicesChanged:)
        name:QTCaptureDeviceWasDisconnectedNotification
        object:nil];
  }
  return self;
}
- (void)dealloc {
  [[NSNotificationCenter defaultCenter] removeObserver:self];
  [super dealloc];
}
- (void)onDevicesChanged:(NSNotification *)notification {
  manager_->OnDevicesChange();
}
@end

namespace Sourcey {
namespace Media {

void* CreateDeviceWatcherCallback(DeviceManager* dm) {
  return [[DeviceWatcherImpl alloc] init:dm];
}
void ReleaseDeviceWatcherCallback(void* watcher) {
  DeviceWatcherImpl* watcher_impl = static_cast<DeviceWatcherImpl*>(watcher);
  [watcher_impl release];
}

bool GetQTKitVideoDevices(std::vector<Device>& devices) {
  NSAutoreleasePool* pool = [[NSAutoreleasePool alloc] init];

  NSArray* qt_capture_devices =
      [QTCaptureDevice inputDevicesWithMediaType:QTMediaTypeVideo];
  NSUInteger count = [qt_capture_devices count];
  LOG(LS_INFO) << count << " capture device(s) found:";
  for (NSUInteger i = 0; i < count; ++i) {
    QTCaptureDevice& qt_capture_device = [qt_capture_devices objectAtIndex:i];

    static const NSString* kFormat = @"localizedDisplayName: \"%@\", "
        "modelUniqueID: \"%@\", uniqueID \"%@\", isConnected: %d, isOpen: %d, "
        "isInUseByAnotherApplication: %d";
    NSString* info = [NSString stringWithFormat:kFormat,
        [qt_capture_device localizedDisplayName],
        [qt_capture_device modelUniqueID],
        [qt_capture_device uniqueID],
        [qt_capture_device isConnected],
        [qt_capture_device isOpen],
        [qt_capture_device isInUseByAnotherApplication]];
    LOG(LS_INFO) << [info cStringUsingEncoding:NSUTF8StringEncoding];

    std::string name([[qt_capture_device localizedDisplayName]
                         cStringUsingEncoding:NSUTF8StringEncoding]);
    devices.push_back(Device(name,
       [[qt_capture_device uniqueID]
           cStringUsingEncoding:NSUTF8StringEncoding]));
  }

  [pool drain];
  return true;
}

} } // namespace Sourcey::Media
