# TODO

Signal move - for copy packet emitter
Double check WebRTC samples
	- add some style to demos
	- take some screenshots for README
Describe how to build WebRTC for use with LibSourcey in README
Improve av module documentation
Replace the use of CMake LibSourcey_XXX variables and set_target_xxx function to set dependencies on a target specific basis
Fix deprecated ICE code
STUN buffer appending extra bytes
CShange socket return values to accomodate -1
Cleanup SocketAdapter ref map
Fix signal atomic copy
ScoketEmitter can't snatch the receiver .. need a shared pointer


# Tests

Test fixtures should be installed relative to compiled test binaries
Integrate socketIO and Symple tests with `make check` (based on nodejs availability)
Video encoder test changing input resolution
Stress test AsyncLoggerWriter