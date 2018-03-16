#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 8
#define audioInput A5

TMRpcm audio;
bool recMode = 0;
bool playMode = 0;
bool recordingExists = 0;
char filename[] = "1.wav";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  audio.speakerPin = 9;
  pinMode(audioInput,INPUT);
  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), recordButton, RISING);
  attachInterrupt(digitalPinToInterrupt(3), playbackButton, RISING);
  SD.begin(SD_ChipSelectPin);
  audio.CSPin = SD_ChipSelectPin;
  audio.setVolume(7);
  Serial.println("Initialization complete");
}

void loop() {
  // put your main code here, to run repeatedly:

}

void recordButton() {
  if(recMode == 0) {
    recMode = 1;
    Serial.println("Begin Recording");
    audio.startRecording(filename, 16000, audioInput);
  }
  else {
    recMode = 0;
    audio.stopRecording(filename);
    Serial.println("End Recording");
  }
}

void playbackButton() {
  if (audio.isPlaying() == 0) {
    recordingExists = SD.exists(filename);
    if (recordingExists) {
      Serial.println("Recording Exists");
      Serial.println("Begin Playback");
      audio.play(filename);  
    }
    else {
      Serial.println("File does not exist");
    }
  }
  else {
    audio.disable();
    Serial.println("Playback Stopped");
  }
}

