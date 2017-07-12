/*
 MIDI note player

 This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
 If this circuit is connected to a MIDI synth, it will play
 the notes F#-0 (0x1E) to F#-5 (0x5A) in sequence.


 The circuit:
 * digital in 1 connected to MIDI jack pin 5
 * MIDI jack pin 2 connected to ground
 * MIDI jack pin 4 connected to +5V through 220-ohm resistor
 Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

 created 13 Jun 2006
 modified 13 Aug 2012
 by Tom Igoe

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/Midi

 */

#define SWITCH_A 8
#define SWITCH_B 9
#define SWITCH_C 10
#define SWITCH_D 11

boolean switchA = false;
boolean switchB = false;
boolean switchC = false;
boolean switchD = false;

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(115200);

  // Setup inputs
  pinMode(SWITCH_A, INPUT);
  pinMode(SWITCH_B, INPUT);
  pinMode(SWITCH_C, INPUT);
  pinMode(SWITCH_D, INPUT);
}

void checkButton(int switchPin, boolean &switchState) {
  boolean newVal = digitalRead(switchPin);

  if (newVal != switchState) {
    if(newVal) {
      noteOn(153, (0x40 + switchPin), newVal ? 0x80 : 0x00);
    } else {
      noteOn(137, (0x40 + switchPin), newVal ? 0x80 : 0x00);
    }
    switchState = newVal;
  }
}

void loop() {
  //playScale();
  checkButton(SWITCH_A, switchA);
  checkButton(SWITCH_B, switchB);
  checkButton(SWITCH_C, switchC);
  checkButton(SWITCH_D, switchD);

  delay(50);
}

void playScale() {
  // play notes from F#-0 (0x1E) to F#-5 (0x5A):
  for (int note = 0x1E; note < 0x5A; note ++) {
    //Note on channel 1 (0x90), some note value (note), middle velocity (0x45):
    noteOn(0x90, note, 0x45);
    delay(100);
    //Note on channel 1 (0x90), some note value (note), silent velocity (0x00):
    noteOn(0x90, note, 0x00);
    delay(100);
  }
}

//  plays a MIDI note.  Doesn't check to see that
//  cmd is greater than 127, or that data values are  less than 127:
void noteOn(int cmd, int pitch, int velocity) {
  Serial.write(cmd);
  Serial.write(pitch);
  Serial.write(velocity);
}

