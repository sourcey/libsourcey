var http = require('http')
var server = http.createServer(function (req, res) {
  req.pipe(res);
});
server.listen(8000);
