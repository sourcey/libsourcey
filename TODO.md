# TODO

Refactor Timer to use async pattern
Add move arguments to packetstream for inline packet args
Describe how to build WebRTC for use with LibSourcey in README
Improve av module documentation
Support multicore servers starting with `httpechoserver` sample
Default NullLogger
No Console output for Release by default
Stress test AsyncLoggerWriter
Add http::ConnectionStream with PacketStream IO and fix http::Form


# Tests

Test fixtures should be installed relative to compiled test binaries
Integrate TURN tests with `make check`
Video encoder test changing input resolution


# Maybe

Replace zlib with libarchive for better archive support

# HTTP Server

Optimize the HTTP server
	- store incoming sockets in a list and listen to first request
	- parse incoming headers and create connection from request type (ws or standard) or close on error
	- emit connection when created and headers parsed
	- connection emits payload and complete
