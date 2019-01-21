/*
  neopixelmusic

  Version 1.0
  December 11, 2016
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

// ********** NeoPixel **********

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEO_PIXELS, PIN_NEO, NEO_GRB + NEO_KHZ800);


void triColor(uint8_t wait, int position, boolean blink){

  for (int i=0; i < strip.numPixels(); i++) {
    switch((i+position) % 3) {
      case 0:
        // white
        strip.setPixelColor(i, 255,255,255);
        break;
      case 1:
        // red
        strip.setPixelColor(i, 255,0,0);
        break;
      case 2:
        // green
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


// ********** Music **********

// ********** We wish you a merry xmas **********
// * Wartz
// **********
int melody[] = {
  NOTE_D5,
  NOTE_G5, NOTE_G5, NOTE_A5, NOTE_G5, NOTE_FS5,
  NOTE_E5, NOTE_E5, NOTE_E5,
  NOTE_A5, NOTE_A5, NOTE_B5, NOTE_A5, NOTE_G5,
  NOTE_FS5, NOTE_D5, NOTE_D5,
  NOTE_B5, NOTE_B5, NOTE_C6, NOTE_B5, NOTE_A5,
  NOTE_G5, NOTE_E5, NOTE_D5,
  NOTE_E5, NOTE_A5, NOTE_FS5,
  NOTE_G5
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
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

void playMelodyWithLED(){
    for (int thisNote = 0; thisNote < 29; thisNote++) {

        // to calculate the note duration, take one second
        // divided by the note type.
        //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
        int noteDuration = 1000 / noteDurations[thisNote];

        if (noteDurations[thisNote] == 4) {
          noteDuration = 1000 / 2.8;
        }

        tone(PIN_SPK, melody[thisNote], noteDuration);

        // change light
        triColor(100,thisNote % 3, false);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        int pauseBetweenNotes = noteDuration * 0.65;
        delay(pauseBetweenNotes);
        // stop the tone playing:
        noTone(PIN_SPK);
    }
}

int val = 0;     // variable for reading the pin status

//int p = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);  // declare LED as output
  pinMode(PIN_BTN, INPUT);    // declare pushbutton as input

  strip.begin();
  strip.setBrightness(12);
  strip.show(); // Initialize all pixels to 'off'
}

void loop(){
  //triColor(100);

  for (int i = 0; i < 3; i++) {
    triColor(100,i,true);
  }

  val = digitalRead(PIN_BTN);  // read input value
  if (val == HIGH) {         // check if the input is HIGH (button released)
    digitalWrite(PIN_LED, LOW);  // turn LED OFF
  } else {
    digitalWrite(PIN_LED, HIGH);  // turn LED ON
    playMelodyWithLED();
  }
}
