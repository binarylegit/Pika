
/*
 * Copyright (c) 2014 Daniel Fowler
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <Wire.h>
#include <LSM303.h>
#include <Servo.h> 

int rightSpeedPWMPin = 5;
int leftSpeedPWMPin = 6;
int rightDirectionPin = 7;
int leftDirectionPin = 8;

int servoPositionPWMPin = 11;

int rangefinderAnalogPin = 3; // analog pin 3
int rangefinderMaxThreshold = 500; // at this value stop

int buzzerDigitalPin = 3;
int userButton = 12;
int yellowLED = 13;

int batteryLevelVoltageDividerAnalogPin = 1; // analog pin 1

// Compass, and Accelerometer
LSM303 compass;
char compassReport[80]; // for debugging

// Servo
Servo neckServo;
int neckServoPosition = 0;

// Motor forward or reverse
int FORWARD = LOW;
int REVERSE = HIGH;

// Serial debugging:
int baudrate = 9600;


void setup() {
  
//  setupMotors();
  
  setupSerial();
  
//  setupServo();

//  setupBuzzer();

  setupCompass();

  analogReference(DEFAULT);

}

void setupCompass() {
  Wire.begin();
  compass.init();
  compass.enableDefault();
}

void setupBuzzer() {
  pinMode(buzzerDigitalPin, OUTPUT);
}

void setupServo() {
  neckServo.attach(servoPositionPWMPin);
}

void setupSerial() {
  Serial.begin(baudrate);
  Serial.flush();
}

void setupMotors() {
      // set pin modes
  pinMode(rightDirectionPin, OUTPUT);
  pinMode(leftDirectionPin, OUTPUT);
  pinMode(rightSpeedPWMPin, OUTPUT);
  pinMode(leftSpeedPWMPin, OUTPUT);

    // initialize to forward stopped
  digitalWrite(rightDirectionPin, FORWARD);
  digitalWrite(leftDirectionPin, FORWARD);
  analogWrite(rightSpeedPWMPin, 0);
  analogWrite(leftSpeedPWMPin, 0);
  
}

// LOOP ////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  
  serialDebug();
  
//  testRangeFinderStop();

//  testServo();

// testBuzzer();

}

void serialDebug() {
  // range finder
  int rangeVal = analogRead(rangefinderAnalogPin);
  Serial.print("Range: ");
  Serial.println(rangeVal, DEC);
  
  // battery level
  int batteryLevel = analogRead(batteryLevelVoltageDividerAnalogPin);
  Serial.print("Battery Level: ");
  Serial.println(batteryLevel, DEC);

  // Compass and accelerometer
  compass.read();
  snprintf(compassReport, sizeof(compassReport), "A: %6d %6d %6d    M: %6d %6d %6d",
    compass.a.x, compass.a.y, compass.a.z,
    compass.m.x, compass.m.y, compass.m.z);
  Serial.println(compassReport);
  
  
  delay(500);
}

void testRangeFinderStop() {
    ///////////////////////////////////////////////////// DRIVE
  digitalWrite(rightDirectionPin, FORWARD);
  digitalWrite(leftDirectionPin, FORWARD);
  analogWrite(rightSpeedPWMPin, 50);
  analogWrite(leftSpeedPWMPin, 50);

  delay(500);
  
  boolean isWithinMaxRange = false;
  do {
    int currentRange = analogRead(rangefinderAnalogPin);
    if(currentRange >= rangefinderMaxThreshold) {
      isWithinMaxRange = true;
    }
  } while(!isWithinMaxRange);
  
  // Forward Stopped
  digitalWrite(rightDirectionPin, FORWARD);
  digitalWrite(leftDirectionPin, FORWARD);
  analogWrite(rightSpeedPWMPin, 0);
  analogWrite(leftSpeedPWMPin, 0);
  
    delay(10000); // 10 seconds
  
}

void testServo() {
  
  for(neckServoPosition = 0; neckServoPosition < 180; neckServoPosition += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    neckServo.write(neckServoPosition);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
  for(neckServoPosition = 180; neckServoPosition>=1; neckServoPosition-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    neckServo.write(neckServoPosition);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  
  neckServoPosition = 90;
  neckServo.write(neckServoPosition);

  delay(1000);

}

void testBuzzer() { 
  tone(buzzerDigitalPin, 500);
  delay(500);
  noTone(buzzerDigitalPin);
  delay(500);
}
