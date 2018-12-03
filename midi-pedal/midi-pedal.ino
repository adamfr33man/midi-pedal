  /*
  This has an I2C LCD and a bunch of switches

  Wiring
  ------
  4 - LCD SDA
  5 - LCD SCL

  Top Row switches
  10 - A
  9  - B
  8  - C
  7  - D

  Middle switches
  6  - E
  5  - F

  Bottom switches
  16 - G
  15 - H
  14 - I
  4  - J

  Volume Pedal
  18 - Switch Down
  19 - Switch Up
  20 - Volume
 */
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "MIDIUSB.h"

#define SWITCH_A 10
#define SWITCH_B 9
#define SWITCH_C 8
#define SWITCH_D 7

#define SWITCH_E 5
#define SWITCH_F 6

#define SWITCH_G 16
#define SWITCH_H 15
#define SWITCH_I 14
#define SWITCH_J 4

#define SWITCH_DOWN A0
#define SWITCH_UP   A1

#define VOLUME_PEDAL A2

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

boolean switchDown = false;
boolean switchUp = false;

uint8_t volume = 0;

boolean cc_mode = true;

LiquidCrystal_I2C lcd(0x3F, 16, 2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int midiMap[] = {0x0B, 0xB0, 0x45, 0x00};

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

  pinMode(SWITCH_DOWN, INPUT);
  pinMode(SWITCH_UP, INPUT);

  pinMode(VOLUME_PEDAL, INPUT);

  lcd.print("Midi Pedal is");
  lcd.setCursor(0, 1);
//  Serial.write("EEPROM length is: ", EEPROM.length());
}

void checkButton(int switchPin, boolean &switchState) {
  boolean newVal = digitalRead(switchPin);

  if (newVal != switchState) {
    sendMidi({0x0B, 0xB0 | 0x99, 0x45, switchToNote(switchPin)});
    switchState = newVal;
  }
}

void checkCCButton(int switchPin, boolean &switchState) {
  boolean newVal = digitalRead(switchPin);

  if (newVal != switchState) {
    sendMidi({0x0B, 0xB0 | 0x99, switchToNote(switchPin), 0x01});
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

  checkCCButton(SWITCH_E, switchE);
  lcd.setCursor(4, 1);
  lcd.print(switchE ? "1" : "0");
  checkCCButton(SWITCH_F, switchF);
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

//  checkButton(SWITCH_DOWN, switchDown);
//  lcd.setCursor(8, 1);
//  lcd.print(switchDown ? "1" : "0");
//  checkButton(SWITCH_UP, switchUp);
//  lcd.setCursor(9, 1);
//  lcd.print(switchUp ? "1" : "0");

  readVolume();

  MidiUSB.flush();
  delay(10);
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
      return 0x0D;
    break;

    case SWITCH_F:
      return 0x0E;
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

    case SWITCH_UP:
      return 8;
    break;

    case SWITCH_DOWN:
      return 9;
    break;

  }
}

void sendMidi(midiEventPacket_t noteOn) {
  Serial.write(noteOn.header);
  Serial.write(",");
  Serial.write(noteOn.byte1);
  Serial.write(",");
  Serial.write(noteOn.byte2);
  Serial.write(",");
  Serial.write(noteOn.byte3);
  Serial.write("\n");

  MidiUSB.sendMIDI(noteOn);
}

void readVolume() {
  uint8_t newVolume;
  newVolume = map(analogRead(VOLUME_PEDAL) / 4, 245, 35, 0, 127);
  if(newVolume > volume + 1 || newVolume < volume - 1) {
    Serial.print("Volume: ");
    Serial.println(newVolume);
    volume = newVolume;
    sendMidi({0x0B, 0xB0 | 0x99, 0x0C, volume});
  }
}
