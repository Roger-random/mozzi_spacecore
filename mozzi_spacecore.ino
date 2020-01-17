/*
 * Sketch using a modified Mozzi SampleHuffman class to play an audio sample
 * at pseudorandom time intervals.
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

boolean playing;
unsigned long nextPlay;

void setup() {
  // Set analong zero to be input reading ambient noise for random seed.
  pinMode(A0, INPUT);

  // Start running Mozzi, but don't start playing sound yet.
  startMozzi();
  playing = false;

  // Read two analog values from A0, use the second as random seed.
  // I don't understand why so many Arduino sketches throw away the first read, but
  // if there's a good reason I'm following suit. If there's no good reason... well
  // it doesn't delay my program that much.
  analogRead(0);
  randomSeed(analogRead(0));

  // Play once immediately so we know audio circuit works.
  nextPlay = millis();
}

// If playing, stop playing if sample is complete.
// If not playing, start playing if pin is held low.
void updateControl(){
  if (playing) {
    if (spacecore.complete()) {
      playing = false;
      // Randomly choose amount of time before next play
      nextPlay = millis() + random( 15*1000, 120*1000 );
    }
  } else {
    if (millis() > nextPlay) {
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
