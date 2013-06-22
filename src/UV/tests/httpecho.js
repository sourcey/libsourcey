var net = require('net');
var server = net.createServer(function(c) { //'connection' listener
  console.log('Client connected');
  c.on('end', function() {
    console.log('Client disconnected');
  });
  c.write('hello\r\n');
  c.pipe(c);
});
server.listen(1337, function() { //'listening' listener
  console.log('Server listening on', 1337);
});