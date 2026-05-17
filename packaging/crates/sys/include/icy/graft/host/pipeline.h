///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @defgroup grafthostpipeline Graft host pipeline C surface
///
/// Narrow host-exported C ABI for the first Rust-facing pipeline surface.
/// @{


#pragma once


#include "icy/graft/abi.h"

#include <stdint.h>


#define ICY_PIPELINE_API_ABI_VERSION 1u
#define ICY_PIPELINE_API_MANIFEST_SYMBOL "icy_pipeline_api_manifest"
#define ICY_PIPELINE_API_ENTRYPOINT "icy_pipeline_api"


ICY_GRAFT_EXTERN_C_BEGIN


extern const icy_graft_manifest_t icy_pipeline_api_manifest;


typedef struct icy_pipeline icy_pipeline_t;


typedef struct icy_pipeline_options
{
    uint32_t struct_size;
    const char* name;
    const char* signalling_token;
    const char* room;
    const char* ice_server;
} icy_pipeline_options_t;


typedef struct icy_pipeline_api
{
    uint32_t abi_version;
    uint32_t struct_size;

    icy_graft_status_t (*create)(const icy_pipeline_options_t* options, icy_pipeline_t** out_pipeline);
    void (*destroy)(icy_pipeline_t* pipeline);

    icy_graft_status_t (*attach_rtsp_source)(icy_pipeline_t* pipeline, const char* url);
    icy_graft_status_t (*attach_webrtc_sender)(icy_pipeline_t* pipeline,
                                               const char* signalling_url,
                                               const char* peer_id);

    icy_graft_status_t (*start)(icy_pipeline_t* pipeline);
    icy_graft_status_t (*stop)(icy_pipeline_t* pipeline);

    const char* (*last_error)(const icy_pipeline_t* pipeline);
} icy_pipeline_api_t;


typedef const icy_pipeline_api_t* (*icy_pipeline_api_get_t)(void);


const icy_pipeline_api_t* icy_pipeline_api(void);


ICY_GRAFT_EXTERN_C_END


/// @}
