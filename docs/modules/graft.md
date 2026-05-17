# Graft

> Native ABI contracts, host C surfaces, and runtime loading helpers

**[API Reference →](../api/graft.md)** · **[Source →](https://github.com/nilstate/icey/tree/main/src/graft/)** · **CMake target:** `icey::graft`

## Overview

The `graft` module is icey's native ABI boundary. It defines the manifest a shared library or host surface must export, the runtime kinds icey understands, and the small C++ loader that opens plugin libraries, validates their manifests, and resolves their declared entrypoint.

`graft` is intentionally narrow. It does not prescribe a C++ object model across the ABI boundary. Instead, the host and the plugin agree on a manifest plus one typed C entrypoint. That keeps the boundary explicit, versioned, and easier to validate at load time.

The same ABI primitives also cover host-out C surfaces. A binding such as `icey-sys` can bind to a host-exported function table through a per-surface manifest and entrypoint without inventing a Rust-specific shim.

Combined with `pacm`, the two modules cover the full lifecycle: `pacm` downloads and installs plugin archives, and `graft` loads those installed libraries into the process and hands the host the plugin entrypoint it asked for.

**Dependencies:** `base`

## Architecture

The plugin-in runtime contract looks like this:

```text
Host process                         Plugin shared library
──────────────────────────────────   ───────────────────────────────────
graft::Library::open(path)       →   shared library loaded
load manifest symbol             →   icy_graft_manifest exported
validateManifest(...)            →   ABI, id, name, version, runtime checked
entrypoint<GetApiFunc>()         →   named exported function resolved
api = getApi()                   →   host uses the plugin's typed API
```

Host-out surfaces use the same bytes with the polarity reversed:

```text
Foreign binding / process             icey shared library
──────────────────────────────────   ───────────────────────────────────
resolve surface manifest symbol   →   icy_pipeline_api_manifest exported
validate manifest                 →   ABI, id, name, version, runtime checked
resolve manifest.entrypoint       →   icy_pipeline_api exported
api = getApi()                    →   binding uses the typed C table
```

Four pieces make up the shared contract:

- `graft::ABI_VERSION` — incremented when the binary contract changes.
- `graft::Manifest` / `icy_graft_manifest_t` — metadata exported from a plugin or host surface.
- `graft::RuntimeKind` — currently `native`, `worker`, or `host`.
- A typed entrypoint function name stored in the manifest and resolved by `graft::Library`.

## Usage

### Define the shared API header

The host and plugin both include the same API header. That header defines the function table and the entrypoint name the manifest will advertise.

```cpp
#pragma once

#include <icy/graft/graft.h>

namespace myapp::plugin {

struct Api
{
    void (*setValue)(const char* value);
    const char* (*value)();
};

using GetApiFunc = const Api* (*)();

inline constexpr const char* ENTRYPOINT = "myapp_plugin_api";

} // namespace myapp::plugin
```

### Export the manifest from the plugin

The plugin implements its API and exports both the manifest and the entrypoint.

```cpp
#include "mypluginapi.h"

namespace {

void setValue(const char* value) { /* ... */ }
const char* value() { /* ... */ return ""; }

const myapp::plugin::Api API = {
    setValue,
    value,
};

} // namespace

ICY_GRAFT_PLUGIN("my-plugin",
                 "My Plugin",
                 "1.0.0",
                 icy::graft::RUNTIME_NATIVE,
                 myapp::plugin::ENTRYPOINT)

extern "C" ICY_GRAFT_EXPORT const myapp::plugin::Api* myapp_plugin_api()
{
    return &API;
}
```

The manifest is exported as `icy_graft_manifest`. `graft::Library` reads that symbol first, validates it, then resolves the entrypoint named by `manifest().entrypoint`.

### Load the plugin in the host

```cpp
#include <icy/graft/graft.h>
#include "mypluginapi.h"

icy::graft::Library library;
library.open("./libmyplugin.so");

if (icy::graft::parseRuntimeKind(library.manifest().runtime) !=
    icy::graft::RuntimeKind::Native) {
    throw std::runtime_error("unexpected runtime kind");
}

auto getApi = library.entrypoint<myapp::plugin::GetApiFunc>();
const myapp::plugin::Api* api = getApi();
api->setValue("ready");
```

If the manifest is missing required fields, the ABI version does not match, or the runtime string is unknown, `library.open()` throws and closes the library again before returning control to the caller.

### Export a host-out C surface

Host surfaces include the C ABI primitives, choose per-surface symbols, and mark the manifest with the `host` runtime. They do not use the plugin manifest symbol `icy_graft_manifest`.

```cpp
#include <icy/graft/host/pipeline.h>

namespace {

const icy_pipeline_api_t API = {
    ICY_PIPELINE_API_ABI_VERSION,
    sizeof(icy_pipeline_api_t),
    createPipeline,
    destroyPipeline,
    attachRtspSource,
    attachWebRtcSender,
    startPipeline,
    stopPipeline,
    lastError,
};

} // namespace

ICY_GRAFT_HOST_SURFACE(icy_pipeline_api_manifest,
                       "icy.pipeline",
                       "icey Pipeline API",
                       "0.1.0",
                       ICY_PIPELINE_API_ENTRYPOINT)

extern "C" ICY_GRAFT_EXPORT const icy_pipeline_api_t* icy_pipeline_api()
{
    return &API;
}
```

The graft manifest ABI version only covers the manifest layout. Every host-out function table must also carry its own `abi_version` and `struct_size` fields so bindings can reject incompatible table layouts before calling through function pointers.

The first concrete host-out surface is `icy_pipeline_api`, exported by the
`icey::pipeline_capi` target. It owns a narrow RTSP-to-browser pipeline and
uses `icey::webrtc_support` internally:

- `create` / `destroy` manage an opaque `icy_pipeline_t`
- `create` accepts an `icy_pipeline_options_t`; callers must set
  `struct_size`, and may pass `name`, `signalling_token`, `room`, and
  `ice_server`
- `attach_rtsp_source` stores the RTSP URL to open through `av::MediaCapture`
- `attach_webrtc_sender` configures Symple signalling; pass `ws://host:port`
  or `wss://host:port`, and use `peer_id` as the local Symple user id
- `start` opens the media source, starts a private libuv loop, joins the
  configured Symple room, accepts incoming calls, and begins streaming when
  the WebRTC session reaches `Active`
- `stop` tears down the stream, session, client, and private loop
- `last_error` returns the most recent pipeline error string, valid until the
  next call that mutates that pipeline

Current host-out conventions:

- one manifest symbol per public surface, such as `icy_pipeline_api_manifest`
- one entrypoint symbol per public surface, such as `icy_pipeline_api`
- opaque handles across the ABI, never C++ object pointers with public layout
- explicit create/destroy pairs for owned handles
- status-code returns for fallible calls; no C++ exceptions across the C ABI
- string lifetime documented by the owning surface

### Runtime kinds

`graft` currently defines three runtime strings:

- `graft::RUNTIME_NATIVE` for code loaded directly into the current process.
- `graft::RUNTIME_WORKER` for plugin contracts intended to execute in a worker runtime.
- `graft::RUNTIME_HOST` for host-exported C ABI surfaces.

Hosts can use `parseRuntimeKind()` and `runtimeKindName()` to normalize and report that value instead of string-comparing ad hoc.

`graft::Library` is still the plugin loader. It rejects `host` manifests even though the generic `validateManifest()` helper accepts them; use `validateHostSurfaceManifest()` when validating a per-surface host manifest.

## Configuration

There is no runtime configuration surface for `graft`. The compatibility boundary is the manifest and the exported entrypoint:

- keep `graft::ABI_VERSION` in sync between host and plugin
- choose a stable entrypoint name and publish it in the shared API header
- reject manifests whose runtime kind does not match the host's execution model
- version each host-out function table independently with `abi_version` and `struct_size`

## See Also

- [Pacm](pacm.md) — package download and installation flow for plugin bundles
- [Base](base.md) — `SharedLibrary`, platform macros, and runtime utilities underneath `graft`
