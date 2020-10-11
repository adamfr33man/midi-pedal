#include <string.h>

#include <AceButton.h>
using namespace ace_button;

#include "MIDIUSB.h"

#include "WebUSB.h"
WebUSB WebUSBSerial(1 /* https:// */, "webusb.github.io/arduino/demos/console");

#define MainSerial WebUSBSerial
//#define MainSerial Serial

AceButton upButton(A0);
AceButton downButton(A1);

void handleEvent(AceButton*, uint8_t, uint8_t);

int pedalPosition  = 0;

void setup() {
  // Setup WebUSB
  MainSerial.begin(115200);
  MainSerial.write("Sketch begins.\n");
  MainSerial.flush();
   
  // Setup Serial port for debugging
  Serial.begin(115200);
  Serial.write("Sketch begins.\n");
  Serial.flush();

  ButtonConfig* buttonConfig = ButtonConfig::getSystemButtonConfig();
  buttonConfig->setEventHandler(handleEvent);
//  buttonConfig->setFeature(ButtonConfig::kFeatureClick);
//  buttonConfig->setFeature(ButtonConfig::kFeatureDoubleClick);
//  buttonConfig->setFeature(ButtonConfig::kFeatureLongPress);
//  buttonConfig->setFeature(ButtonConfig::kFeatureRepeatPress);
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT);
  upButton.setEventHandler(handleEvent);
  downButton.setEventHandler(handleEvent);

  pedalPosition = readVolumePedal();
}

void loop() {
  int newReading = readVolumePedal();

  if((newReading - pedalPosition) > 1 || (pedalPosition - newReading) > 1) {
    Serial.println("Pedal: " + String(newReading));
    Serial.flush();
    pedalPosition = newReading;
    sendMIDI(newReading);
  }

  upButton.check();
  downButton.check();
}

void handleEvent(AceButton* button, uint8_t eventType,
    uint8_t /*buttonState*/) {
  String buttonLabel = button->getPin() == 18 ? "Up" : "Down";
  
  switch (eventType) {
    case AceButton::kEventPressed:
      Serial.println(buttonLabel + " button pressed");
      
      sendMIDI(button->getPin());
      break;
    case AceButton::kEventReleased:
      Serial.println(buttonLabel + " button released");

      sendMIDI(button->getPin());
      break;
//    case AceButton::kEventDoubleClicked:
//      Serial.println(button->getPin() + " button double clicked");
//      break;
//    case AceButton::kEventLongPressed:
//      Serial.println(button->getPin() +  " button long pressed");
//      break;
//    case AceButton::kEventRepeatPressed:
//      Serial.println(button->getPin() + " button repeat");
//      break;
  }
  Serial.flush();
}

int readVolumePedal() {
  const int lower = 141;
  const int upper = 997;

  int rawValue = analogRead(A2);

  return map(rawValue, lower, upper, 127, 0);
}

int sendMIDI(int velocity) {
  int cmd = 0x0B;
  int channel = 0xB0 | 0x99;
  int pitch = 0x45;
      
  midiEventPacket_t midi = {cmd, channel, pitch, velocity};
  MidiUSB.sendMIDI(midi);
  MidiUSB.flush();  
}
