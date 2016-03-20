var WebSocketServer = require('ws').Server
  , wss = new WebSocketServer({port: 1337});
wss.on('connection', function(ws) {
    console.log('connected');
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
