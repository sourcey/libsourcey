var http = require('http');

var options = {
  host: '127.0.0.1',
  port: 1337,
  path: '/foo.html'
};

http.get(options, function(resp){
  resp.on('data', function(chunk){
    //do something with chunk
    console.log("Got response: " + chunk);
  });
}).on("error", function(e){
  console.log("Got error: " + e.message);
});