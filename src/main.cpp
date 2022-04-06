#include <Arduino.h>
#include <FastLED.h>

/*
  Ultrasonic Sensor HC-SR04 and Arduino Tutorial

  by Dejan Nedelkovski,
  www.HowToMechatronics.com
*/

// defines pins numbers
#define led_pin 3
#define trigPinR 4
#define echoPinR 5
#define trigPinL 6
#define echoPinL 7

// How many leds in your strip?
#define NUM_LEDS 32
#define BRIGHTNESS 50

// ideale afstand
#define AfstandMinimum 5    //Alles kleiner wordt 5cm
#define AfstandMaximum 100  //Alles groter wordt 100cm
#define AfstandIdealVan 35  // Hiertussen is goed geparkeerd
#define AfstandIdealTot 45

// defines variables
long duration;
int distanceL, distanceR;
int DistParking;   // DELETEME DELETEME
int ledL, ledR;

CRGB leds[NUM_LEDS];

void setup()
{
  pinMode(trigPinL, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinL, INPUT);  // Sets the echoPin as an Input
  pinMode(trigPinR, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPinR, INPUT);  // Sets the echoPin as an Input
  Serial.begin(9600);       // Starts the serial communication

  FastLED.addLeds<NEOPIXEL, led_pin>(leds, NUM_LEDS);  // GRB ordering is assumed
  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();  // clear all pixel data
}

void BerekenLED(int distance, int ledOK, int ledVer) {
  int led ;
  if (distance <= AfstandMinimum) {  //Veel te kortbij
    leds[ledVer] = CRGB::Red;
  }
  else if (distance < AfstandIdealVan) {  // Te kortbij
    led = map(distance, AfstandMinimum, AfstandIdealVan, ledVer, ledOK);
    leds[led] = CRGB::Red;
  }
  else if (distance <= AfstandIdealTot) { //Ideaal
    leds[ledOK] = CRGB::Green;
  }
  else if (distance < AfstandMaximum) {  // Te ver
    led = map(distance, AfstandIdealTot, AfstandMaximum, ledOK, ledVer);
    leds[led] = CRGB::Blue;
  }
  else {  //Veel te ver
    leds[ledVer] = CRGB::Blue;
  }
}

void loop() {
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
  BerekenLED(distanceL, NUM_LEDS/2-1, 0);
  BerekenLED(distanceR, NUM_LEDS/2, NUM_LEDS);
  FastLED.show();

  Serial.print(" LedL: ");
  Serial.print(ledL);
  Serial.print(" - LedR: ");
  Serial.println(ledR);
  
}