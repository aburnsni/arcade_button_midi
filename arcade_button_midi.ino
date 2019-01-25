#include <MIDI.h>
#include "midi_notes.h"

const int buttons = 4;

//MIDI Setup
int* song[buttons] = {NOTE_C2, NOTE_E2, NOTE_C3, NOTE_GS2};
int midiChannel[buttons] = {6, 6, 6, 6}; // midi channel for each button
int instruments[16] = {102, 999, 999, 999, 999, 999, 999, 999, 999, 999 /*Drums*/, 999, 999, 999, 999, 999, 999};
MIDI_CREATE_INSTANCE(HardwareSerial, Serial, MIDI);  


const int buttonPin[buttons] = {2, 3, 4, 5};
int buttonState[buttons] = {1, 1, 1, 1};
const int ledPin[buttons] = {6, 9, 10, 11};  // Using PWM pins on arduino nano
const int powerled = 12;

bool playing[buttons] = {false, false, false, false};  //Is note currently playing
unsigned long lasttrig[buttons];
unsigned long debounce = 50;

void setup() {
  MIDI.begin();
  delay(200);

  // MIDIsoftreset();  // Midi Reset
  // delay(200);
  // MIDIinstrumentset();  // Set intruments for MIDI channels
  // delay(200);

  // Setup I/Os
  pinMode(powerled, OUTPUT);
  digitalWrite(powerled, HIGH);
  for (int i = 0; i < buttons; i++)
  {
    pinMode(buttonPin[i], INPUT_PULLUP);
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], LOW);
  }

  for (int i = 0; i < buttons; i++) {
  lasttrig[i] = millis();
}
}

void loop() {
  for (uint8_t i = 0; i < buttons; i++) {
    buttonState[i] = digitalRead(buttonPin[i]);
    if ((buttonState[i] == LOW) && (playing[i] == false) && (millis() - lasttrig[i] > debounce)) {
      // turn LED on:
      digitalWrite(ledPin[i], HIGH);
      MIDI.sendNoteOn(song[i], 100, midiChannel[i]);
      playing[i] = true;
      lasttrig[i] = millis();
    } else if ((buttonState[i] == HIGH) && (playing[i] == true)) {
      // turn LED off:
      digitalWrite(ledPin[i], LOW);
      MIDI.sendNoteOff(song[i], 100, midiChannel[i]);
      playing[i] = false;
    }
  }
  delay(1);
}

void MIDIsoftreset()  // switch off ALL notes on channel 1 to 16
{
  for (int channel = 0; channel < 16; channel++)
  {
    MIDI.sendControlChange(123, 0, channel);
  }
}

void MIDIinstrumentset() {
  for (uint8_t i = 0; i < 16; i++) {  // Set instruments for all 16 MIDI channels
    if (instruments[i] < 128) {
      MIDI.sendProgramChange(instruments[i], i + 1);
    }
  }
}
