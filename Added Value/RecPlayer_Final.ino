#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>

#define SD_ChipSelectPin 8
#define audioInput A5

TMRpcm audio;
bool recMode = 0;
bool finishedPlayback = 0;
bool recordingExists = 0;
char filename[] = "1.wav";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  audio.speakerPin = 9;
  pinMode(audioInput,INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), recordButton, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), playbackButton, CHANGE);
  SD.begin(SD_ChipSelectPin);
  audio.CSPin = SD_ChipSelectPin;
  audio.setVolume(6);
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
      if (finishedPlayback == 0){
        finishedPlayback = 1;
        Serial.println("Recording Exists");
        Serial.println("Begin Playback");
        audio.play(filename);  
      }
      else {
        finishedPlayback = 0;
      }
    }
    else {
      Serial.println("File does not exist");
      finishedPlayback = !finishedPlayback;
    }
  }
  else {
    audio.disable();
    Serial.println("Playback Stopped");
    finishedPlayback = 0;
  }
}

