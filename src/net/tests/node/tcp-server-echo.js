var net = require('net');

net.createServer(function(c) {
  console.log('client connected');
  c.on('end', function() {
    console.log('client disconnected');
  });
  c.pipe(c);
}).listen(1337, function() {
  console.log('server listening on', 1337);
});
