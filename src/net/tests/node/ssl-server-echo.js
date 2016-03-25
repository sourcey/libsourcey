var tls = require('tls'),
    fs = require('fs');

var options = {
  key: fs.readFileSync('../key.pem'),
  cert: fs.readFileSync('../cert.pem')
};

tls.createServer(options, function (s) {
  console.log('client connected');
  s.on('end', function() {
    console.log('client disconnected');
  });
  s.pipe(s);
}).listen(1338, function() {
  console.log('server listening on', 1338);
});
