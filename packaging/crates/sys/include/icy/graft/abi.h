///
//
// icey
// Copyright (c) 2005, icey <https://0state.com>
//
// SPDX-License-Identifier: LGPL-2.1+
//
/// @defgroup graftabi Graft C ABI primitives
///
/// C-compatible manifest and export conventions shared by plugin-in and
/// host-out graft surfaces.
/// @{


#pragma once


#include <stdint.h>


#if defined(_WIN32)
#define ICY_GRAFT_EXPORT __declspec(dllexport)
#elif defined(__GNUC__) || defined(__clang__)
#define ICY_GRAFT_EXPORT __attribute__((visibility("default")))
#else
#define ICY_GRAFT_EXPORT
#endif


#ifdef __cplusplus
#define ICY_GRAFT_EXTERN_C_BEGIN extern "C" {
#define ICY_GRAFT_EXTERN_C_END }
#define ICY_GRAFT_MANIFEST_STORAGE ICY_GRAFT_EXPORT extern const
#else
#define ICY_GRAFT_EXTERN_C_BEGIN
#define ICY_GRAFT_EXTERN_C_END
#define ICY_GRAFT_MANIFEST_STORAGE ICY_GRAFT_EXPORT const
#endif


#define ICY_GRAFT_ABI_VERSION 1u

#define ICY_GRAFT_PLUGIN_MANIFEST_SYMBOL "icy_graft_manifest"

#define ICY_GRAFT_RUNTIME_NATIVE "native"
#define ICY_GRAFT_RUNTIME_WORKER "worker"
#define ICY_GRAFT_RUNTIME_HOST "host"


typedef struct icy_graft_manifest
{
    uint32_t abiVersion;
    const char* fileName;
    const char* id;
    const char* name;
    const char* version;
    const char* runtime;
    const char* entrypoint;
} icy_graft_manifest_t;


typedef enum icy_graft_status
{
    ICY_GRAFT_STATUS_OK = 0,
    ICY_GRAFT_STATUS_ERROR = 1,
    ICY_GRAFT_STATUS_INVALID_ARGUMENT = 2,
    ICY_GRAFT_STATUS_INVALID_STATE = 3,
    ICY_GRAFT_STATUS_UNSUPPORTED = 4,
} icy_graft_status_t;


#define ICY_GRAFT_MANIFEST(symbolName, surfaceId, surfaceName, surfaceVersion, runtimeKind, entrypointName) \
    ICY_GRAFT_EXTERN_C_BEGIN                                                                      \
    ICY_GRAFT_MANIFEST_STORAGE icy_graft_manifest_t symbolName = {                                \
        ICY_GRAFT_ABI_VERSION,                                                                    \
        __FILE__,                                                                                 \
        surfaceId,                                                                                \
        surfaceName,                                                                              \
        surfaceVersion,                                                                           \
        runtimeKind,                                                                              \
        entrypointName,                                                                           \
    };                                                                                            \
    ICY_GRAFT_EXTERN_C_END


#define ICY_GRAFT_HOST_SURFACE(manifestSymbol, surfaceId, surfaceName, surfaceVersion, entrypointName) \
    ICY_GRAFT_MANIFEST(manifestSymbol,                                                              \
                       surfaceId,                                                                   \
                       surfaceName,                                                                 \
                       surfaceVersion,                                                              \
                       ICY_GRAFT_RUNTIME_HOST,                                                       \
                       entrypointName)


/// @}
