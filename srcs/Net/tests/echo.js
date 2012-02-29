var net = require('net');

var server = net.createServer(function (socket) {
  console.log("got conn");
  socket.write("Echo server\r\n");
  socket.pipe(socket);
	
  // TODO: Announce or get booted
  setInterval(function () {
  	console.log("sendingn");
  	socket.write("Echo server\r\n");
  }, 100);
});

server.listen(1337, "127.0.0.1");