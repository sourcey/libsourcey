# icey

Rust bindings for [icey](https://0state.com/icey), a C++ media
runtime: WebRTC, FFmpeg, and async networking in one toolkit. No Google
monolith. No dependency hell. No fighting three build systems to get a frame
on screen.

The Rust surface is in early development. The first public API is a narrow
RTSP-to-WebRTC pipeline wrapper over the graft C ABI.

## Crate Layout

Most Rust users should depend on this crate. It wraps the raw C ABI exposed by
`icey-sys`, keeps unsafe calls behind a small Rust API, and owns the public
examples.

The companion `icey-sys` crate is published separately because it owns native
link metadata and generated FFI bindings. Keep it as an implementation layer
unless you need direct access to the graft function table.

## Native Link

Build icey's native `pipeline_capi` target, then point Cargo at the produced
library directory:

```sh
cmake --build build-pipeline-api --target pipeline_capi -j 4
ICEY_LIB_DIR=/path/to/libdir cargo test -p icey
```

On macOS, set `DYLD_LIBRARY_PATH` to the same directory when running examples
or tests that load `libicy_pipeline_capi.dylib`.

## Example

```sh
ICEY_LIB_DIR=/path/to/libdir \
DYLD_LIBRARY_PATH=/path/to/libdir \
cargo run -p icey --example rtsp_to_browser -- \
  rtsp://127.0.0.1:8554/demo ws://127.0.0.1:4500 icey-rust
```

## Resources

- Product: https://0state.com/icey
- Documentation: https://0state.com/icey/docs
- Source: https://github.com/nilstate/icey
- Rust crate: https://crates.io/crates/icey
- Raw sys crate: https://crates.io/crates/icey-sys
- Releases: https://github.com/nilstate/icey/releases
- Docker: `docker run --rm --network host 0state/icey-server:latest`

## License

LGPL-2.1-or-later, matching the parent project.
