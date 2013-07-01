var tls = require('tls'),
    fs = require('fs')//,
    //colors = require('colors'),
    //msg = [
            ".-..-..-.  .-.   .-. .--. .---. .-.   .---. .-.",
            ": :; :: :  : :.-.: :: ,. :: .; :: :   : .  :: :",
            ":    :: :  : :: :: :: :: ::   .': :   : :: :: :",
            ": :: :: :  : `' `' ;: :; :: :.`.: :__ : :; ::_;",
            ":_;:_;:_;   `.,`.,' `.__.':_;:_;:___.':___.':_;" 
          //].join("\n"); //.cyan;

var options = {
  key: fs.readFileSync('private-key.pem'),
  cert: fs.readFileSync('public-cert.pem')
};

tls.createServer(options, function (s) {
  console.log('Client connected');
  s.on('end', function() {
    console.log('Client disconnected');
  });
  //s.on('data', function(data) { 
  //  console.log('Client recv: ', data.length);
  //  s.write('You said: ' + data.length); // + data.length
  //});
  //s.write(msg+"\n");
  s.pipe(s);
  //s.close();
}).listen(1338, function() {
  console.log('Server listening on', 1338);
});