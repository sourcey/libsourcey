# TODO

Double check WebRTC samples
	- take some screenshots for README
	- rename samples with server suffix
    - input file option
    - add test video to repo
    - apply video transformations
    - repeat timestamps

STUN buffer appending extra bytes ... add test to check empty buffer at end of read
Improve av module documentation
Describe how to build WebRTC for use with LibSourcey in README 
	- maybe move to article and link
	- tell macos users to build webrtc from source
Replace the use of CMake LibSourcey_XXX variables and set_target_xxx function to set dependencies on a target specific basis
Fix av mediaserver
Fix deprecated ICE code
Add move interface to packets


# Tests

Test fixtures should be installed relative to compiled test binaries
Integrate socketIO and Symple tests with `make check` (based on nodejs availability)
Video encoder test changing input resolution
Stress test AsyncLoggerWriter

# WebRTC SFU server

https://tools.ietf.org/html/rfc7667
https://github.com/ibc/mediasoup
https://thenewdialtone.com/the-end-of-transcoding-webrtc-video-sessions/
https://bloggeek.me/webrtc-sfu-challenges/


