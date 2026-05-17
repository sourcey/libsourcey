#![allow(non_camel_case_types)]
#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(improper_ctypes)]
//! Raw FFI bindings for [icey](https://0state.com/icey).

include!(concat!(env!("OUT_DIR"), "/bindings.rs"));

pub const ICEY_GRAFT_ABI_VERSION: u32 = ICY_GRAFT_ABI_VERSION;
pub const ICEY_PIPELINE_API_ABI_VERSION: u32 = ICY_PIPELINE_API_ABI_VERSION;

pub const ICEY_GRAFT_STATUS_OK: icy_graft_status_t = icy_graft_status_ICY_GRAFT_STATUS_OK;
pub const ICEY_GRAFT_STATUS_ERROR: icy_graft_status_t = icy_graft_status_ICY_GRAFT_STATUS_ERROR;
pub const ICEY_GRAFT_STATUS_INVALID_ARGUMENT: icy_graft_status_t =
    icy_graft_status_ICY_GRAFT_STATUS_INVALID_ARGUMENT;
pub const ICEY_GRAFT_STATUS_INVALID_STATE: icy_graft_status_t =
    icy_graft_status_ICY_GRAFT_STATUS_INVALID_STATE;
pub const ICEY_GRAFT_STATUS_UNSUPPORTED: icy_graft_status_t =
    icy_graft_status_ICY_GRAFT_STATUS_UNSUPPORTED;
