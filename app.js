
/**
 * Module dependencies.
 */

var express = require('express');
var routes = require('./routes');
var user = require('./routes/user');
var http = require('http');
var path = require('path');
var serialport = require('serialport');

var app = express();

// all environments
app.set('port', process.env.PORT || 3000);
app.set('views', __dirname + '/views');
app.set('view engine', 'jade');
app.use(express.favicon());
app.use(express.logger('dev'));
app.use(express.bodyParser());
app.use(express.methodOverride());
app.use(app.router);
app.use(express.static(path.join(__dirname, 'public')));

// development only
if ('development' == app.get('env')) {
  app.use(express.errorHandler());
}

app.get('/', routes.index);
app.get('/users', user.list);

var server = http.createServer(app);

//
// serialport
//
//var portName = '/dev/tty.usbmodemfa141';
var portName = '/dev/tty.usbmodem1411';
var sp = new serialport.SerialPort(portName, {
    baudRate: 9600
    // , dataBits: 8
    // parity: 'none',
    // stopBits: 1,
    // flowControl: false,
    , parser: serialport.parsers.readline("\n")
});

sp.on("open", function () {
    console.log('open');

    sp.on('data', function(data) {
        console.log('data received: ' + data);
    });
});

sp.on('error', function(err) {
  console.log('err ' + err);
});

//
// socket.io
//
var io = require('socket.io').listen(server);

io.sockets.on("connection", function(socket) {
    console.log('socket connected');

    socket.on('mode change', function(_mode){
        var mode = 'mc' + String(_mode);
        console.log('mode changed : ', _mode);
        sp.write(mode, function(err, results) {
            if(err) console.log(err);
        });
        sp2.write(mode, function(err, results) {
            if(err) console.log(err);
        });
    });
});

//
// serialport1
//
var portName2 = '/dev/cu.usbmodem1421';
var sp2 = new serialport.SerialPort(portName2, {
    baudRate: 9600
    // , dataBits: 8
    // parity: 'none',
    // stopBits: 1,
    // flowControl: false,
    , parser: serialport.parsers.readline("\n")
});

sp2.on("open", function () {
    console.log('open');

    sp2.on('data', function(data) {
        console.log('data received: ' + data);
    });
});

sp2.on('error', function(err) {
  console.log('err ' + err);
});

server.listen(app.get('port'), function(){
  console.log('Express server listening on port ' + app.get('port'));
});
