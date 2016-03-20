var fs = require('fs');

var options = {
  key: fs.readFileSync('../private-key.pem'),
  cert: fs.readFileSync('../public-cert.pem')
};

var app = require('https').createServer(options);
app.listen(1338, function() {
  console.log('server listening on', 1338);
});
var WebSocketServer = require('ws').Server;
var wss = new WebSocketServer({server: app}); //, wss = new WebSocketServer({port: 1337});
wss.on('connection', function(ws) {
    console.log('ws connection');

    ws.on('message', function(message) {
        console.log('received: %s', message);
        ws.send(message);
    });

    ws.on('error', function(error, code) {
      console.log('ws error: ' + error + ': ' + code);
    });
});
wss.on('error', function(error, code) {
  console.log('wss error: ' + error + ': ' + code);
});
