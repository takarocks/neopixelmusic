/*
  neopixelmusic

  Version 1.1
  December 25, 2020
  Taka kitazume

  Tutorials
  https://www.arduino.cc/en/Tutorial/Tone
  https://www.arduino.cc/en/Tutorial/PlayMelody
 */

#include <Adafruit_NeoPixel.h>
#include "pitches.h"
#include "Volume3.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NEO_PIXELS 150

#define PIN_NEO 10
#define PIN_LED 12
#define PIN_BTN 8
#define PIN_SPK 9

#define TIME_LED_ON  100
#define TIME_LED_OFF 50

// * MUSIC TOTAL NOTES *
#define B

#define NOTESTOTAL1 29
#define NOTESTOTAL2 41

// ********************
// * Global Variables *
// ********************

int song = 1;       // initial song number
int button = 0;     // variable for reading the pin status

// ************
// * NeoPixel *
// ************
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEO_PIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);

void triColor(uint8_t wait, int position, boolean blink){

  for (int i=0; i < strip.numPixels(); i++) {
    switch((i+position) % 3) {
      case 0:
        // white 255,255,255
        strip.setPixelColor(i, 255,255,255);
        break;
      case 1:
        // red 255,0,0
        strip.setPixelColor(i, 255,0,0);
        break;
      case 2:
        // green 0,255,0
        strip.setPixelColor(i, 0,255,0);
        break;
    }
  }

  strip.show(); // Light up

  delay(TIME_LED_ON);

  // Blink only in normal loop
  if(blink){

    for (int i=0; i < strip.numPixels(); i=i+3) {
      strip.setPixelColor(i+position, 0);        //turn every third pixel off
    }

    strip.show();

    delay(TIME_LED_OFF);

  }
}

// *********
// * MUSIC *
// *********

// *************************************
// * Music 1: We wish you a merry xmas *
// *************************************
int melody1[NOTESTOTAL1] = {
  N_D5,
  N_G5, N_G5, N_A5, N_G5, N_FS5,
  N_E5, N_E5, N_E5,
  N_A5, N_A5, N_B5, N_A5, N_G5,
  N_FS5, N_D5, N_D5,
  N_B5, N_B5, N_C6, N_B5, N_A5,
  N_G5, N_E5, N_D5,
  N_E5, N_A5, N_FS5,
  N_G5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int note1[NOTESTOTAL1] = {
  4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8, 8, 8,
  4, 4, 4,
  4, 8, 8 ,8, 8,
  4, 4, 4,
  4, 4, 4,
  1
};

// ******************************
// * Music 2: Jingle Bells Rock *
// ******************************
int melody2[NOTESTOTAL2] = {
  N_C6, N_C6, N_C6, N_B5, N_B5, N_B5,
  N_A5, N_B5, N_A5, N_E5,
  N_A5, N_B5, N_A5, N_E5, N_G5,
  N_A5, N_B5, N_A5, N_F5,
  N_D5, N_E5, N_F5, N_G5, N_A5, N_G5,
  N_D5, N_E5, N_F5, N_G5,
  N_Z, N_A5, N_A5, N_G5, N_A5, N_G5, N_A5, N_G5,
  N_A5, N_G5, N_A5, N_E5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int note2[NOTESTOTAL2] = {
  6, 10, 4, 6, 10, 4,
  6, 10, 4, 1,
  6, 10, 4, 4, 4,
  6, 10, 4, 1,
  6, 4, 8, 6, 4, 8,
  6, 8, 6, 1,
  4, 8, 6, 10, 6, 10, 6, 10,
  6, 10, 6, 1
};

// *********************
// * playMelodyWithLED *
// *********************
void playMelodyWithLED(int total, int melody[], int note[]){
    for (int i = 0; i < total; i++) {
        // Calculating the note duration by dividing 1000 milliseconds (1 second) by the note duration.
        // e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        // NOTE, Need some adjustment in some duration.

        int duration = 0;
        switch(note[i]) {
          case 1:
            duration = 1000 / note[i];
            break;
          case 2:
            duration = 1000 / note[i];
            break;
          case 4:
            duration = 1000 / 2.8;
            break;
          case 6:
            duration = 1000 / 5.2;
            break;
          case 8:
            duration = 1000 / note[i];
            break;
          case 10:
            duration = 1000 / 11.6;
            break;
        }

        if(note[i] == N_Z) {
          delay(duration);
        } 
        else {
          tone(PIN_SPK, melody[i], duration);
        }

        // change light
        triColor(100,i % 3, false);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = duration * 0.65;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(PIN_SPK);
    }
}

void setup() {
  pinMode(PIN_LED, OUTPUT);  // declare LED as output
  pinMode(PIN_BTN, INPUT);    // declare pushbutton as input

  strip.begin();
  strip.setBrightness(12);
  strip.show(); // Initialize all pixels to 'off'
}

void loop(){
  for (int i = 0; i < 3; i++) {
    triColor(100,i,true);
  }

  button = digitalRead(PIN_BTN);  // read input button
  if (button == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(PIN_LED, LOW);  // turn LED OFF
  } else {
    digitalWrite(PIN_LED, HIGH);  // turn LED ON

    if (song == 1) {
      playMelodyWithLED(NOTESTOTAL1, melody1, note1);
      song = 2;
    } else if (song == 2) {
      playMelodyWithLED(NOTESTOTAL2, melody2, note2);
      song = 1;
    }
  }
}
