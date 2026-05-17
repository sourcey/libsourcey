#![cfg(icey_native_link)]

use icey::{Error, Pipeline, PipelineOptions, Status};

#[test]
fn dogfoods_pipeline_wrapper_against_native_c_api() {
    icey::validate_native_api().expect("native pipeline API should validate");

    let mut pipeline = Pipeline::new(
        PipelineOptions::new()
            .name("rust-dogfood")
            .signalling_token("token")
            .room("dogfood")
            .ice_server("stun:127.0.0.1:3478"),
    )
    .expect("pipeline should be created");

    let err = pipeline.start().expect_err("start requires an RTSP source");
    assert_native_status(err, Status::InvalidState);
    assert!(
        pipeline
            .last_error()
            .is_some_and(|message| message.contains("RTSP source")),
        "expected native last_error to mention the missing RTSP source"
    );

    let err = pipeline
        .attach_rtsp_source("")
        .expect_err("empty RTSP URL should be rejected");
    assert_native_status(err, Status::InvalidArgument);

    let err = pipeline
        .attach_webrtc_sender("ws://127.0.0.1:notaport", "peer")
        .expect_err("invalid signalling port should be rejected");
    assert_native_status(err, Status::InvalidArgument);

    pipeline
        .attach_rtsp_source("rtsp://127.0.0.1:8554/demo")
        .expect("RTSP URL should be accepted before start");
    pipeline
        .attach_webrtc_sender("ws://127.0.0.1:4500", "peer")
        .expect("signalling endpoint should be accepted before start");
    pipeline
        .stop()
        .expect("stop before start should be a no-op");
}

fn assert_native_status(error: Error, expected: Status) {
    match error {
        Error::NativeStatus { status, .. } => assert_eq!(status, expected),
        other => panic!("expected native status {expected:?}, got {other:?}"),
    }
}
