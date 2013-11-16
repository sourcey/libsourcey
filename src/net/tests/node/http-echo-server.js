var http = require('http');
 
http.createServer(function (request, response) {

    /*
    response.setHeader('Content-Type', 'text/html; charset=UTF-8');
    response.setHeader('Transfer-Encoding', 'chunked');
 
    response.write("Hello client");
 
    // this is another chunk of data sent to a client after 2 seconds before the
    // 5-second chunk is sent.
    var count = 0;
    setInterval(function(){
        //console.log('interval')
        //response.write("chunk " + count++);
 
    }, 2000);
    */
 
}).listen(1337, null);

/*
var http = require('http')
var server = http.createServer(function (req, res) {
  //req.pipe(res);
  console.log('on connection');
  
  req.on('data', function (chunk) {
    console.log('BODY: ' + chunk);
  });
  
  res.on('data', function (chunk) {
    console.log('BODY: ' + chunk);
  });
  
  //res.writeHead(200, {'Content-Type': 'text/plain'}); 
  res.write('Hello Node!'); 
  res.write('Hello Node!'); 
  res.write('Hello Node!'); 
  res.write('Hello Node!'); 
  res.write('Hello Node!'); 
  res.write('Hello Node!'); 
  //res.end('Hello Node!\n'); 
});
server.listen(1337);
*/


/*
var http = require('http')
  , fs = require('fs');

fs.readFile('image.jpg', function(err, data) {
  if (err) throw err; // Fail if the file can't be read.
  http.createServer(function(req, res) {
    res.writeHead(200, {'Content-Type': 'image/jpeg'});
    res.end(data); // Send the file data to the browser.
  }).listen(1337);
  console.log('Server running at http://localhost:8124/');
});
*/