# icey-sys

Raw Rust FFI bindings for [icey](https://0state.com/icey).

This crate is the generated `bindgen` layer over icey's graft C ABI. Most Rust
users should depend on the safe `icey` crate instead.

## Native Link Contract

`icey-sys` owns the Cargo `links = "icey_pipeline_capi"` contract for
`libicy_pipeline_capi`. Set `ICEY_LIB_DIR` to the directory containing that
library when building runnable examples or tests.

The crate carries the public graft C headers needed for packaging and docs.rs
builds. In a source checkout, `build.rs` prefers the repo headers so local ABI
edits are reflected immediately.

## Resources

- Product: https://0state.com/icey
- Documentation: https://0state.com/icey/docs
- Source: https://github.com/nilstate/icey
- Rust crate: https://crates.io/crates/icey
- Raw sys crate: https://crates.io/crates/icey-sys

## License

LGPL-2.1-or-later, matching the parent project.
