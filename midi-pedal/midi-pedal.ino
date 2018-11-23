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
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include "MIDIUSB.h"

#define VOLUME_PEDAL 20
#define BOOT_COUNT 0
#define MIDI_MAP_START 16

byte boot_count;
int switches[] = {10, 9, 8, 7, 6, 5, 16, 15, 14, 4, 18, 19};

boolean switchState[sizeof(switches)];

typedef struct {
  uint8_t cmd;
  uint8_t channel;
  uint8_t pitch;
  uint8_t velocity;
} midi_packet;

#define PACKET_SIZE sizeof(midi_packet)

midi_packet midiValues[] = {
  {0x0B, 0xB0 | 0x99, 0x45, 0x00},
  {0x0B, 0xB0 | 0x99, 0x45, 0x01},
  {0x0B, 0xB0 | 0x99, 0x45, 0x02},
  {0x0B, 0xB0 | 0x99, 0x45, 0x03},
  {0x0B, 0xB0 | 0x99, 0x45, 0x08},
  {0x0B, 0xB0 | 0x99, 0x45, 0x09},
  {0x0B, 0xB0 | 0x99, 0x45, 0x04},
  {0x0B, 0xB0 | 0x99, 0x45, 0x05},
  {0x0B, 0xB0 | 0x99, 0x45, 0x06},
  {0x0B, 0xB0 | 0x99, 0x45, 0x07},
  {0x0B, 0xB0 | 0x99, 0x45, 0x0A},
  {0x0B, 0xB0 | 0x99, 0x45, 0x0B}
};

LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  boot_count = EEPROM.read(BOOT_COUNT);
  //  Set MIDI baud rate:
  Serial.begin(115200);

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  delay(250);

  // Setup inputs
  for (int i = 0; i < sizeof(switches); i++) {
    pinMode(switches[i], INPUT);
    switchState[i] = digitalRead(switches[i]);
  }
  
  pinMode(VOLUME_PEDAL, INPUT);

  lcd.print("Midi Pedal is");
  lcd.setCursor(0, 1);

  Serial.println("MIDI Pedal Ready.\n");

  Serial.print("Boot Count: ");
  Serial.print(boot_count, DEC);
  Serial.println();

  EEPROM.write(BOOT_COUNT, ++boot_count);
  //writeMidiMap();
  readMidiMap();
}

void checkButton(int switchPin, boolean &switchState) {
  boolean newVal = digitalRead(switchPin);

  if (newVal != switchState) {
    sendMidi(midiValues[switchPin]);
    switchState = newVal;
  }
}

void loop() {
  int pin;
  for(int i = 0; i < sizeof(switches); i++) {
    checkButton(switches[i], switchState[i]);
    lcd.setCursor(i, 1);
    lcd.print(switchState[i] ? "1" : "0");
  }

  readVolume();

  MidiUSB.flush();
  delay(50);
}

void sendMidi(midi_packet packet) {
  Serial.write(packet.pitch);
  Serial.write(": ");
  Serial.write(packet.velocity > 10 ? "On" : "Off");
  Serial.write("\n");

  midiEventPacket_t midi = {packet.cmd, packet.channel, packet.pitch, packet.velocity};

  MidiUSB.sendMIDI(midi);  
}

void readVolume() {
  uint8_t volume;
  volume = analogRead(VOLUME_PEDAL);
}

void writeMidiMap() {
  for(int i = 0; i < sizeof(midiValues) / PACKET_SIZE; i++) {
    int address = MIDI_MAP_START + (i * sizeof(midi_packet));

    EEPROM.put(address, midiValues[i]);
  }
}

void readMidiMap() {
  for(int i = 0; i < sizeof(midiValues) / PACKET_SIZE; i++) {
    int address = MIDI_MAP_START + (i * PACKET_SIZE);

    midi_packet data;
    EEPROM.get(address, data);

    Serial.print("Midi code for ");
    Serial.print(i, DEC);
    Serial.print(" is ");
    Serial.print(data.cmd, DEC);
    Serial.print(",");
    Serial.print(data.channel, DEC);
    Serial.print(",");
    Serial.print(data.pitch, DEC);
    Serial.print(",");
    Serial.print(data.velocity, DEC);
    Serial.println("");
  }
  Serial.println("\n");
}
