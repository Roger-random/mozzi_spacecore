/*
 * Sketch using a modified Mozzi SampleHuffman class to play an audio sample
 * at the press of the button (when pin D4 is held low.)
 * 
 * Audio is a version of "SPAAACE!" from Valve Software's Portal 2.
 * Downloaded from https://theportalwiki.com/wiki/Core_voice_lines
 * Downsampled using Audacity to fit within flash memory of an Arduino
 * 
 * Mozzi library released by Tim Barrass under CC-BY-NC-SA
 * Audio sample property of Valve Software, Inc.
 * This sketch released by Roger Cheng under MIT License
 */

#include <MozziGuts.h>
#include "SampleHuffman.h"
#include "spacewav.h"

SampleHuffman spacecore(space_core_SOUNDDATA,space_core_HUFFMAN,space_core_SOUNDDATA_BITS);

#define PLAY_PIN 4

boolean playing;

void setup() {
  // Configure button pin for input with internal pullup resistor.
  pinMode(PLAY_PIN, INPUT_PULLUP);

  // Start running Mozzi, but don't start playing sound yet.
  startMozzi();
  playing = false;
}

// If playing, stop playing if sample is complete.
// If not playing, start playing if pin is held low.
void updateControl(){
  if (playing) {
    if (spacecore.complete()) {
      playing = false;
    }
  } else {
    if (digitalRead(PLAY_PIN) == LOW) {
      // Since we don't check again until sample is done, there is no need to debounce
      spacecore.start();
      playing = true;
    }
  }
}

// If playing, return next audio sample.
// Otherwise return silence.
int updateAudio(){
  if (playing) {
    return spacecore.next();
  } else {
    return 0;
  }
}


void loop() {
  audioHook();
}
