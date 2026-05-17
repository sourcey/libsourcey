//! # icey
//!
//! Rust bindings for [icey](https://0state.com/icey), a C++ media
//! runtime that brings WebRTC, FFmpeg, and async networking into one toolkit.

use std::ffi::{CStr, CString, NulError};
use std::fmt;
use std::marker::PhantomData;
use std::ptr;
use std::rc::Rc;

pub use icey_sys as sys;

pub type Result<T> = std::result::Result<T, Error>;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum Status {
    Ok,
    Error,
    InvalidArgument,
    InvalidState,
    Unsupported,
    Unknown(sys::icy_graft_status_t),
}

impl Status {
    fn from_raw(raw: sys::icy_graft_status_t) -> Self {
        match raw {
            sys::ICEY_GRAFT_STATUS_OK => Self::Ok,
            sys::ICEY_GRAFT_STATUS_ERROR => Self::Error,
            sys::ICEY_GRAFT_STATUS_INVALID_ARGUMENT => Self::InvalidArgument,
            sys::ICEY_GRAFT_STATUS_INVALID_STATE => Self::InvalidState,
            sys::ICEY_GRAFT_STATUS_UNSUPPORTED => Self::Unsupported,
            other => Self::Unknown(other),
        }
    }

    fn is_ok(self) -> bool {
        self == Self::Ok
    }
}

#[derive(Debug)]
pub enum Error {
    InteriorNul {
        field: &'static str,
        source: NulError,
    },
    IncompatibleNativeApi(&'static str),
    MissingFunction(&'static str),
    NullHandle,
    NativeStatus {
        status: Status,
        message: Option<String>,
    },
}

impl fmt::Display for Error {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        match self {
            Self::InteriorNul { field, .. } => {
                write!(f, "{field} contains an interior NUL byte")
            }
            Self::IncompatibleNativeApi(reason) => write!(f, "incompatible native API: {reason}"),
            Self::MissingFunction(name) => write!(f, "native API is missing function {name}"),
            Self::NullHandle => write!(f, "native API returned a null pipeline handle"),
            Self::NativeStatus { status, message } => {
                if let Some(message) = message {
                    write!(f, "native API returned {status:?}: {message}")
                } else {
                    write!(f, "native API returned {status:?}")
                }
            }
        }
    }
}

impl std::error::Error for Error {}

#[derive(Debug, Clone, Default)]
pub struct PipelineOptions {
    name: Option<String>,
    signalling_token: Option<String>,
    room: Option<String>,
    ice_server: Option<String>,
}

impl PipelineOptions {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn name(mut self, value: impl Into<String>) -> Self {
        self.name = Some(value.into());
        self
    }

    pub fn signalling_token(mut self, value: impl Into<String>) -> Self {
        self.signalling_token = Some(value.into());
        self
    }

    pub fn room(mut self, value: impl Into<String>) -> Self {
        self.room = Some(value.into());
        self
    }

    pub fn ice_server(mut self, value: impl Into<String>) -> Self {
        self.ice_server = Some(value.into());
        self
    }
}

pub struct Pipeline {
    handle: *mut sys::icy_pipeline_t,
    _not_send_or_sync: PhantomData<Rc<()>>,
}

impl Pipeline {
    pub fn new(options: PipelineOptions) -> Result<Self> {
        let api = pipeline_api()?;
        let create = api.create.ok_or(Error::MissingFunction("create"))?;

        let name = optional_cstring("name", options.name.as_deref())?;
        let signalling_token =
            optional_cstring("signalling_token", options.signalling_token.as_deref())?;
        let room = optional_cstring("room", options.room.as_deref())?;
        let ice_server = optional_cstring("ice_server", options.ice_server.as_deref())?;

        let raw_options = sys::icy_pipeline_options_t {
            struct_size: std::mem::size_of::<sys::icy_pipeline_options_t>() as u32,
            name: optional_ptr(&name),
            signalling_token: optional_ptr(&signalling_token),
            room: optional_ptr(&room),
            ice_server: optional_ptr(&ice_server),
        };

        let mut handle = ptr::null_mut();
        let status = unsafe { create(&raw_options, &mut handle) };
        status_to_result(status, None)?;

        if handle.is_null() {
            return Err(Error::NullHandle);
        }

        Ok(Self {
            handle,
            _not_send_or_sync: PhantomData,
        })
    }

    pub fn attach_rtsp_source(&mut self, url: impl AsRef<str>) -> Result<()> {
        let api = pipeline_api()?;
        let attach = api
            .attach_rtsp_source
            .ok_or(Error::MissingFunction("attach_rtsp_source"))?;
        let url = cstring("url", url.as_ref())?;
        let status = unsafe { attach(self.handle, url.as_ptr()) };
        status_to_result(status, self.last_error())
    }

    pub fn attach_webrtc_sender(
        &mut self,
        signalling_url: impl AsRef<str>,
        peer_id: impl AsRef<str>,
    ) -> Result<()> {
        let api = pipeline_api()?;
        let attach = api
            .attach_webrtc_sender
            .ok_or(Error::MissingFunction("attach_webrtc_sender"))?;
        let signalling_url = cstring("signalling_url", signalling_url.as_ref())?;
        let peer_id = cstring("peer_id", peer_id.as_ref())?;
        let status = unsafe { attach(self.handle, signalling_url.as_ptr(), peer_id.as_ptr()) };
        status_to_result(status, self.last_error())
    }

    pub fn start(&mut self) -> Result<()> {
        let api = pipeline_api()?;
        let start = api.start.ok_or(Error::MissingFunction("start"))?;
        let status = unsafe { start(self.handle) };
        status_to_result(status, self.last_error())
    }

    pub fn stop(&mut self) -> Result<()> {
        let api = pipeline_api()?;
        let stop = api.stop.ok_or(Error::MissingFunction("stop"))?;
        let status = unsafe { stop(self.handle) };
        status_to_result(status, self.last_error())
    }

    pub fn last_error(&self) -> Option<String> {
        let api = pipeline_api().ok()?;
        let last_error = api.last_error?;
        let raw = unsafe { last_error(self.handle) };
        if raw.is_null() {
            return None;
        }
        Some(
            unsafe { CStr::from_ptr(raw) }
                .to_string_lossy()
                .into_owned(),
        )
    }
}

impl Drop for Pipeline {
    fn drop(&mut self) {
        if self.handle.is_null() {
            return;
        }

        if let Ok(api) = pipeline_api() {
            if let Some(destroy) = api.destroy {
                unsafe { destroy(self.handle) };
            }
        }

        self.handle = ptr::null_mut();
    }
}

pub fn validate_native_api() -> Result<()> {
    pipeline_api().map(|_| ())
}

fn pipeline_api() -> Result<&'static sys::icy_pipeline_api_t> {
    validate_manifest()?;

    let api = unsafe { sys::icy_pipeline_api() };
    if api.is_null() {
        return Err(Error::IncompatibleNativeApi("pipeline API pointer is null"));
    }

    let api = unsafe { &*api };
    if api.abi_version != sys::ICEY_PIPELINE_API_ABI_VERSION {
        return Err(Error::IncompatibleNativeApi(
            "pipeline API ABI version mismatch",
        ));
    }
    if (api.struct_size as usize) < std::mem::size_of::<sys::icy_pipeline_api_t>() {
        return Err(Error::IncompatibleNativeApi(
            "pipeline API table size mismatch",
        ));
    }

    Ok(api)
}

fn validate_manifest() -> Result<()> {
    let manifest = unsafe { &sys::icy_pipeline_api_manifest };
    if manifest.abiVersion != sys::ICEY_GRAFT_ABI_VERSION {
        return Err(Error::IncompatibleNativeApi("graft ABI version mismatch"));
    }
    if !cstr_equals(manifest.runtime, sys::ICY_GRAFT_RUNTIME_HOST) {
        return Err(Error::IncompatibleNativeApi(
            "pipeline manifest runtime mismatch",
        ));
    }
    if !cstr_equals(manifest.entrypoint, sys::ICY_PIPELINE_API_ENTRYPOINT) {
        return Err(Error::IncompatibleNativeApi(
            "pipeline manifest entrypoint mismatch",
        ));
    }

    Ok(())
}

fn cstr_equals(raw: *const std::os::raw::c_char, expected: &[u8]) -> bool {
    if raw.is_null() {
        return false;
    }

    unsafe { CStr::from_ptr(raw) }.to_bytes_with_nul() == expected
}

fn status_to_result(status: sys::icy_graft_status_t, message: Option<String>) -> Result<()> {
    let status = Status::from_raw(status);
    if status.is_ok() {
        Ok(())
    } else {
        Err(Error::NativeStatus { status, message })
    }
}

fn optional_cstring(field: &'static str, value: Option<&str>) -> Result<Option<CString>> {
    value.map(|value| cstring(field, value)).transpose()
}

fn cstring(field: &'static str, value: &str) -> Result<CString> {
    CString::new(value).map_err(|source| Error::InteriorNul { field, source })
}

fn optional_ptr(value: &Option<CString>) -> *const std::os::raw::c_char {
    value.as_ref().map_or(ptr::null(), |value| value.as_ptr())
}
