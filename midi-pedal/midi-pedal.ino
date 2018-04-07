/*
  This has an I2C LCD and a bunch of switches

  Wiring
  ------
  4 - LCD SDA
  5 - LCD SCL

 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SWITCH_A 16
#define SWITCH_B 15
#define SWITCH_C 14
#define SWITCH_D 4

#define SWITCH_E 10
#define SWITCH_F 9
#define SWITCH_G 8
#define SWITCH_H 7

#define SWITCH_I 6
#define SWITCH_J 5

boolean switchA = false;
boolean switchB = false;
boolean switchC = false;
boolean switchD = false;

boolean switchE = false;
boolean switchF = false;
boolean switchG = false;
boolean switchH = false;

boolean switchI = false;
boolean switchJ = false;

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

  pinMode(SWITCH_E, INPUT);
  pinMode(SWITCH_F, INPUT);
  pinMode(SWITCH_G, INPUT);
  pinMode(SWITCH_H, INPUT);

  pinMode(SWITCH_I, INPUT);
  pinMode(SWITCH_J, INPUT);

  lcd.print("Midi Pedal is");
  lcd.setCursor(0, 1);
//  lcd.print("Ready for action now !");
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
  lcd.setCursor(0, 1);
  lcd.print(switchA ? "1" : "0");
  checkButton(SWITCH_B, switchB);
  lcd.setCursor(1, 1);
  lcd.print(switchB ? "1" : "0");
  checkButton(SWITCH_C, switchC);
  lcd.setCursor(2, 1);
  lcd.print(switchC ? "1" : "0");
  checkButton(SWITCH_D, switchD);
  lcd.setCursor(3, 1);
  lcd.print(switchD ? "1" : "0");

  checkButton(SWITCH_E, switchE);
  lcd.setCursor(4, 1);
  lcd.print(switchE ? "1" : "0");
  checkButton(SWITCH_F, switchF);
  lcd.setCursor(5, 1);
  lcd.print(switchF ? "1" : "0");
  checkButton(SWITCH_G, switchG);
  lcd.setCursor(6, 1);
  lcd.print(switchG ? "1" : "0");
  checkButton(SWITCH_H, switchH);
  lcd.setCursor(7, 1);
  lcd.print(switchH ? "1" : "0");

  checkButton(SWITCH_I, switchI);
  lcd.setCursor(8, 1);
  lcd.print(switchI ? "1" : "0");
  checkButton(SWITCH_J, switchJ);
  lcd.setCursor(9, 1);
  lcd.print(switchJ ? "1" : "0");

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
//  Serial.write(cmd);
//  Serial.write(pitch);
//  Serial.write(velocity);
  Serial.write(pitch);
  Serial.write(": ");
  Serial.write(velocity > 10 ? "On" : "Off");
  Serial.write("\n");
}
