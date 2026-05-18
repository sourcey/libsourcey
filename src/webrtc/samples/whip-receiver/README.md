# WHIP Receiver

Receives WHIP publishers on `/whip` and records each session to
`whip-<session-id>.mp4`. The ingest side expects H.264 video and Opus audio;
the file writer decodes and records MP4 as H.264/AAC.

## Build

Configure icey with WebRTC, FFmpeg, samples, and libdatachannel enabled, then
build the sample target:

```sh
cmake -S . -B build-whip \
  -DWITH_FFMPEG=ON \
  -DWITH_LIBDATACHANNEL=ON \
  -DBUILD_SAMPLES=ON
cmake --build build-whip --target whip-receiver
```

## Local Certificate

Run the helper from the directory where you want `cert.pem` and `key.pem`
written:

```sh
./src/webrtc/samples/whip-receiver/gen-cert.sh
```

## Run

```sh
./build-whip/webrtc/samples/whip-receiver/whip-receiver \
  --host 127.0.0.1 \
  --port 8080 \
  --tls-cert cert.pem \
  --tls-key key.pem \
  --output-dir ./recordings
```

For quick localhost dogfooding, omit `--tls-cert` and `--tls-key` and use
`http://127.0.0.1:8080/whip`.

## Publish With FFmpeg

```sh
ffmpeg -re -f lavfi -i testsrc2=size=1280x720:rate=30 \
  -f lavfi -i sine=frequency=1000:sample_rate=48000 \
  -c:v libx264 -profile:v baseline -tune zerolatency -bf 0 \
  -c:a libopus -ar 48000 -ac 2 \
  -f whip http://127.0.0.1:8080/whip
```

Use `https://127.0.0.1:8080/whip` when running with the generated certificate.
When the receiver is started with `--auth-token`, add `-authorization <token>`
before `-f whip`.

## OBS Studio

Open Settings, then Stream. Select WHIP as the service, set the server to
`https://127.0.0.1:8080/whip`, enter the optional bearer token if the receiver
was started with `--auth-token`, and start streaming.

## Pipeline

```text
WHIP POST/PATCH/DELETE
  -> WhipSignaller
  -> PeerSession recvonly tracks
  -> WebRtcTrackReceiver
  -> VideoDecoder / AudioDecoder
  -> MultiplexPacketEncoder
  -> recordings/whip-<id>.mp4
```

## Limits

The sample is intentionally narrow: one publisher per WHIP session, H.264/Opus
ingest, MP4 file output, no relay mode, no ICE restart handling, and minimal
bearer-token authentication only.
