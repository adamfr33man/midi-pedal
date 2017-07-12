const logger = require('winston-color'),
  midi = require('midi'),
  SerialPort = require('serialport'),
  ByteLength = SerialPort.parsers.ByteLength;

var port = new SerialPort('COM3', {
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
    logger.log('info', `Data: [${parseInt(data[0])}, ${parseInt(data[1])}, ${parseInt(data[2])}]`);
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
output.openPort(3);

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



var input = new midi.input();

// Count the available output ports.
logger.log('info', `Got: ${input.getPortCount()} port(s)`);

// Get the name of a specified output port.
for (let i = 0; i < input.getPortCount(); i++) {
  logger.log('info', `Port # ${i} is named ${input.getPortName(i)}`);
}

input.on('message', function(deltaTime, message) {
  // The message is an array of numbers corresponding to the MIDI bytes:
  //   [status, data1, data2]
  // https://www.cs.cf.ac.uk/Dave/Multimedia/node158.html has some helpful
  // information interpreting the messages.
  logger.log('info', 'm:[' + message + ']');
});

// Open the first available input port.
input.openPort(0);

input.ignoreTypes(false, false, false);