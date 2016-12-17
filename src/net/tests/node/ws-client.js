var WebSocket = require('ws');
var ws = new WebSocket('ws://127.0.0.1:1337/websocket');
ws.on('open', function() {
    ws.send('PING');
    // var array = new Float32Array(5);
    // for (var i = 0; i < array.length; ++i) array[i] = i / 2;
    // ws.send(array, {binary: true, mask: true});
});
ws.on('message', function(data, flags) {
    // flags.binary will be set if a binary data is received
    // flags.masched will be set if the data was masched
    console.log('message', data, flags)
});
ws.on('error', function(error, code) {
    console.log('error: ' + error + ': ' + code);
});
