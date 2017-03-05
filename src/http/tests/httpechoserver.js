var http = require('http')
var server = http.createServer(function (req, res) {
  req.pipe(res);

  // console.log('request', req.url, req.headers)
  // var body = '';
  // req.on('readable', function() {
  //     body += req.read();
  // });
  // req.on('end', function() {
  //     console.log(body);
  // });
}).listen(1337, function (req, res) {
  console.log('Echo server listening on http://localhost:1337')
});
