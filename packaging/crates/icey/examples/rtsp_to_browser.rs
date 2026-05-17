use std::env;
use std::io;

use icey::{Pipeline, PipelineOptions};

fn main() -> Result<(), Box<dyn std::error::Error>> {
    let mut args = env::args().skip(1);
    let Some(rtsp_url) = args.next() else {
        eprintln!(
            "usage: rtsp_to_browser <rtsp-url> [signalling-url] [peer-id]\n\
             env: ICEY_ROOM, ICEY_TOKEN, ICEY_ICE_SERVER"
        );
        std::process::exit(2);
    };

    let signalling_url = args
        .next()
        .unwrap_or_else(|| "ws://127.0.0.1:4500".to_owned());
    let peer_id = args.next().unwrap_or_else(|| "icey-rust".to_owned());
    let room = env::var("ICEY_ROOM").unwrap_or_else(|_| "public".to_owned());
    let ice_server =
        env::var("ICEY_ICE_SERVER").unwrap_or_else(|_| "stun:stun.l.google.com:19302".to_owned());

    let mut options = PipelineOptions::new()
        .name("icey-rust")
        .room(room)
        .ice_server(ice_server);

    if let Ok(token) = env::var("ICEY_TOKEN") {
        if !token.is_empty() {
            options = options.signalling_token(token);
        }
    }

    icey::validate_native_api()?;

    let mut pipeline = Pipeline::new(options)?;
    pipeline.attach_rtsp_source(rtsp_url)?;
    pipeline.attach_webrtc_sender(signalling_url, peer_id)?;
    pipeline.start()?;

    println!("pipeline running; press Enter to stop");
    let mut line = String::new();
    io::stdin().read_line(&mut line)?;
    pipeline.stop()?;

    Ok(())
}
