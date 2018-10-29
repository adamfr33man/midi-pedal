/*
  This has an I2C LCD and a bunch of switches

  Wiring
  ------
  4 - LCD SDA
  5 - LCD SCL

 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MIDIUSB.h"

#define SWITCH_A 10
#define SWITCH_B 9
#define SWITCH_C 8
#define SWITCH_D 7

#define SWITCH_E 6
#define SWITCH_F 5

#define SWITCH_G 16
#define SWITCH_H 15
#define SWITCH_I 14
#define SWITCH_J 4


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

boolean cc_mode = true;

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
      noteOn(153, switchToNote(switchPin), 0x80);
//    if(newVal) {
//      noteOn(153, (0x40 + switchPin), newVal ? 0x80 : 0x00);
//    } else {
//      noteOn(137, (0x40 + switchPin), newVal ? 0x80 : 0x00);
//    }
    switchState = newVal;
  }
}

void loop() {
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

  MidiUSB.flush();
  delay(50);
}

uint8_t switchToNote(int switchPin) {
  switch(switchPin) {
    case SWITCH_A:
      return 0;
    break;

    case SWITCH_B:
      return 1;
    break;
    
    case SWITCH_C:
      return 2;
    break;
    
    case SWITCH_D:
      return 3;
    break;

    case SWITCH_E:
      return 8;
    break;

    case SWITCH_F:
      return 9;
    break;

    case SWITCH_G:
      return 4;
    break;

    case SWITCH_H:
      return 5;
    break;

    case SWITCH_I:
      return 6;
    break;

    case SWITCH_J:
      return 7;
    break;
    
  }
}
 
void noteOn(uint8_t cmd, uint8_t pitch, uint8_t velocity) {
  Serial.write(pitch);
  Serial.write(": ");
  Serial.write(velocity > 10 ? "On" : "Off");
  Serial.write("\n");
  midiEventPacket_t noteOn;
  if(cc_mode) {
    noteOn = {0x0B, 0xB0 | cmd, 0x45, pitch};        
  } else {
    noteOn = {0x09, 0x80 | cmd, 0x40 + pitch, velocity};
  }
  MidiUSB.sendMIDI(noteOn);
}
