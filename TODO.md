# TODO

Update Timestamp.cpp and Timespan.cpp ... move Timeout stuff to util or other? ... move inlines to source file 
Double check WebRTC samples
	- add some style to demos
	- take some screenshots for README
Fix size types - crypto
STUN buffer appending extra bytes ... add test to check empty buffer at end of read
Replace the use of CMake LibSourcey_XXX variables and set_target_xxx function to set dependencies on a target specific basis
Improve av module documentation
Describe how to build WebRTC for use with LibSourcey in README - maybe move to article and link
Fix deprecated ICE code


# Tests

Test fixtures should be installed relative to compiled test binaries
Integrate socketIO and Symple tests with `make check` (based on nodejs availability)
Video encoder test changing input resolution
Stress test AsyncLoggerWriter