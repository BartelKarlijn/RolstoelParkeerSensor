#include <Arduino.h>
#include <FastLED.h>

/*
  Ultrasonic Sensor HC-SR04 and Arduino Tutorial

  by Dejan Nedelkovski,
  www.HowToMechatronics.com
*/

// defines pins numbers
#define trigPinL 6
#define echoPinL 7
#define trigPinR 4
#define echoPinR 5
// defines variables
long duration;
int distanceL, distanceR;

// How many leds in your strip?
#define NUM_LEDS 32
#define DATA_PIN 3
#define BRIGHTNESS 50
CRGB leds[NUM_LEDS];

#define DistParking 40
int ledL, ledR;


void setup()
{
  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinL, INPUT);  // Sets the echoPin as an Input
  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinR, INPUT);  // Sets the echoPin as an Input
  Serial.begin(9600);       // Starts the serial communication

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  // clear all pixel data
}

void loop()
{
  //Links
  delay(100);    //wachten tot andere golf weg is
  // Clears the trigPin
  digitalWrite(trigPinL, LOW);
  delayMicroseconds(5);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinL, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinL, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPinL, HIGH);
  // Calculating the distance
  distanceL = duration * 0.034 / 2;

  //Rechts
  delay(100);    //wachten tot andere golf weg is
  // Clears the trigPin
  digitalWrite(trigPinR, LOW);
  delayMicroseconds(5);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPinR, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinR, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPinR, HIGH);
  // Calculating the distance
  distanceR = duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance in cm L: ");
  Serial.print(distanceL);
  Serial.print(" - R: ");
  Serial.print(distanceR);

// Ledlampje aanzetten
  FastLED.clearData();
  // Links
  if (distanceL >= DistParking) {
    // te ver af, dus Blauw
    ledL = map(min(distanceL, DistParking * 2), DistParking * 2, DistParking, 0, NUM_LEDS/2 -1 );
    leds[ledL] = CRGB::Blue;
  }
  else {
    // te dicht bij, dus Rood
    ledL = map(distanceL, 0, DistParking, 0, NUM_LEDS/2 -1 );
    leds[ledL] = CRGB::Red;
  }

  // Rechts
  if (distanceR >= DistParking) {
    // te ver af, dus Blauw
    ledR = map(min(distanceR, DistParking * 2), DistParking * 2, DistParking, NUM_LEDS-1, NUM_LEDS/2 );
    leds[ledR] = CRGB::Blue;
  }
  else {
    // te dicht bij, dus Rood
    ledR = map(distanceR, 0, DistParking, NUM_LEDS-1, NUM_LEDS/2 );
    leds[ledR] = CRGB::Red;
  }

// Groene lichtjes
  if (  (distanceL <= DistParking + 3)
    and (distanceL >= DistParking - 3)
    and (distanceR <= DistParking + 3)
    and (distanceR >= DistParking - 3) ) {
  
      FastLED.clearData();
      leds[15] = CRGB::Green;
      leds[16] = CRGB::Green;
    }

  Serial.print(" LedL: ");
  Serial.print(ledL);
  Serial.print(" - LedR: ");
  Serial.println(ledR);
  
  FastLED.show();
}