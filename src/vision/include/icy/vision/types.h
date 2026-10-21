///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
///
/// @addtogroup vision
/// @{


#pragma once


#include "icy/json/json.h"
#include "icy/vision/vision.h"

#include <cstdint>
#include <string>


namespace icy {
/// Video analysis for decoded frames: sampling, normalising and detecting, with
/// results reported as vision events.
namespace vision {


/// The frame a result refers to: its source and stream, frame ID, timing, size,
/// pixel format and whether it is a keyframe.
struct Vision_API VisionFrameContext
{
    std::string sourceId;
    std::string streamId;
    uint64_t frameId = 0;
    int64_t ptsUsec = 0;
    int64_t receivedAtUsec = 0;
    int width = 0;
    int height = 0;
    std::string pixelFmt;
    bool keyframe = false;
};


using FrameRef = VisionFrameContext;


/// A rectangle within a frame, in coordinates normalised from 0 to 1.
struct Vision_API Region
{
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;
};


/// Something a detector found in one frame: its label, confidence and region, and
/// the track it belongs to when one is known.
struct Vision_API Detection
{
    std::string label;
    float confidence = 0.0f;
    int64_t trackId = -1;
    Region region;
    json::Value data = json::Value::object();
};


/// An object followed across frames: its track ID, label, confidence and region.
struct Vision_API Track
{
    int64_t trackId = -1;
    std::string label;
    float confidence = 0.0f;
    Region region;
    json::Value data = json::Value::object();
};


} // namespace vision
} // namespace icy


/// @}
