var net = require("net");
var socket = net.createConnection(1337, "127.0.0.1");
socket.setEncoding("utf8");
socket.on("connect", function(){
        console.log("Client connected")
        socket.on("data", function(data){
                console.log("Client data: " + data)
                socket.write(data);
                socket.destroy();
        });
        socket.write("Echo Client\r\n");
});
socket.on("end", function(){
        console.log("Client closed")
        // can be done else where, is similar to http .end("data", encoding);
        socket.end();
        // cleans up the socket.
        socket.destroy();
});