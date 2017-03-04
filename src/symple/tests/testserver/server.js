var Symple = require('symple');
var sy = new Symple();
sy.loadConfig(__dirname + '/' + (process.env.CONFIG ? process.env.CONFIG : 'symple.json'));

// Use environment variables if set
if (process.env.PORT)
  sy.config.port = process.env.PORT;
if (process.env.REDIS_HOST && sy.config.resis)
  sy.config.resis.host = process.env.REDIS_HOST;
if (process.env.REDIS_PORT && sy.config.resis)
  sy.config.resis.port = process.env.REDIS_PORT;

sy.init();

sy.server.on('listening', function(){
  console.log('Symple server listening on port ', sy.server.address().port);
});
