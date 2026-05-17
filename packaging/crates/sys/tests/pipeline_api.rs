#![cfg(icey_native_link)]

use std::ffi::CStr;

use icey_sys::{
    icy_pipeline_api, icy_pipeline_api_manifest, icy_pipeline_api_t, ICEY_GRAFT_ABI_VERSION,
    ICEY_PIPELINE_API_ABI_VERSION, ICY_GRAFT_RUNTIME_HOST, ICY_PIPELINE_API_ENTRYPOINT,
};

#[test]
fn pipeline_api_exports_are_compatible() {
    unsafe {
        assert_eq!(icy_pipeline_api_manifest.abiVersion, ICEY_GRAFT_ABI_VERSION);
        assert_eq!(
            CStr::from_ptr(icy_pipeline_api_manifest.runtime).to_bytes_with_nul(),
            ICY_GRAFT_RUNTIME_HOST
        );
        assert_eq!(
            CStr::from_ptr(icy_pipeline_api_manifest.entrypoint).to_bytes_with_nul(),
            ICY_PIPELINE_API_ENTRYPOINT
        );

        let api = icy_pipeline_api();
        assert!(!api.is_null());
        assert_eq!((*api).abi_version, ICEY_PIPELINE_API_ABI_VERSION);
        assert!((*api).struct_size as usize >= std::mem::size_of::<icy_pipeline_api_t>());
        assert!((*api).create.is_some());
        assert!((*api).destroy.is_some());
        assert!((*api).attach_rtsp_source.is_some());
        assert!((*api).attach_webrtc_sender.is_some());
        assert!((*api).start.is_some());
        assert!((*api).stop.is_some());
        assert!((*api).last_error.is_some());
    }
}
