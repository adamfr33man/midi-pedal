/*
  This has an I2C LCD and a bunch of switches

  Wiring
  ------
  4 - LCD SDA
  5 - LCD SCL

 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SWITCH_A 8
#define SWITCH_B 9
#define SWITCH_C 10
#define SWITCH_D 11

boolean switchA = false;
boolean switchB = false;
boolean switchC = false;
boolean switchD = false;

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  //  Set MIDI baud rate:
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Setup inputs
  pinMode(SWITCH_A, INPUT);
  pinMode(SWITCH_B, INPUT);
  pinMode(SWITCH_C, INPUT);
  pinMode(SWITCH_D, INPUT);

  lcd.print("Midi Pedal is");
  lcd.setCursor(0, 1);
  lcd.print("Ready for action now !");

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

