const logger = require('winston-color'),
  midi = require('midi'),
  SerialPort = require('serialport'),
  ByteLength = SerialPort.parsers.ByteLength;

var port = new SerialPort('COM4', {
  baudRate: 115200,
  autoOpen: false
});

var parser;


port.open(function(err) {
  if (err) {
    logger.log('error', `Error opening port ${err.message}`);
    return;
  }

  parser = port.pipe(new ByteLength({length: 3}));
  // Because there's no callback to write, write errors will be emitted on the port:
  //port.write('main screen turn on');
});

// The open event is always emitted
port.on('open', function() {
  // open logic
  port.on('data', function(data) {
    // logger.log('info', `Data: ${data}`);
    output.sendMessage([data[0], data[1], data[2]]);
  });
});

// Set up a new output.
var output = new midi.output();

// Count the available output ports.
logger.log('info', `Got: ${output.getPortCount()} port(s)`);

// Get the name of a specified output port.
for (let i = 0; i < output.getPortCount(); i++) {
  logger.log('info', `Port # ${i} is named ${output.getPortName(i)}`);
}

// Open the first available output port.
output.openPort(0);

process.stdin.resume();
process.stdin.setEncoding('utf8');
var util = require('util');

process.stdin.on('data', function(text) {
  console.log('received data:', util.inspect(text));
  if (text === 'quit\n' || text === 'quit\r\n') {
    done();
  }
});

function done() {
  console.log('Now that process.stdin is paused, there is nothing more to do.');
  output.closePort();
  process.exit();
}