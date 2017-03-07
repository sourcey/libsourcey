# TODO

Signal move - for copy packet emitter
Double check WebRTC samples
	- add some style to demos
	- take some screenshots for README
Describe how to build WebRTC for use with LibSourcey in README
Improve av module documentation
Replace the use of CMake LibSourcey_XXX variables and set_target_xxx function to set dependencies on a target specific basis
Fix deprecated ICE code
Improve HTP server performance
	- Remove icompare functions from server --- get from parser
	- Remove mutex from NVCollection
	- Test direct 200 close response before handlers are called
	- SocketAdapter::addReceiver - optimize all the += operators


# Tests

Test fixtures should be installed relative to compiled test binaries
Integrate socketIO and Symple tests with `make check` (based on nodejs availability)
Video encoder test changing input resolution
Stress test AsyncLoggerWriter