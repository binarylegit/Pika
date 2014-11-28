
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

// Servo
Servo neckServo;
int neckServoPosition = 90;

// Motor forward or reverse
int FORWARD = LOW;
int REVERSE = HIGH;

// Serial debugging:
int baudrate = 9600;

int compassReadings = 0;
char compassReport[80];

int operatingEnvironment = 0;
boolean isCalibrationCompleted = false;

void setup() {

  setupMotors();
  setAllStop();

  setupSerial();

  setupServo();

  setupBuzzer();

  setupCompass();

  analogReference(DEFAULT);

  operatingEnvironment = 0;
  compassReadings = 0;

  // delay 10 seconds to allow serial monitor to open
  delay(10000);
  setMotorOperatingEnvironment();

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
  neckServo.write(90);
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
}

// LOOP ////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  if(compassReadings <= 5000 && !isCalibrationCompleted){
    compass.read();
    snprintf(compassReport, sizeof(compassReport), "%6d,%6d,%6d", compassReadings, compass.m.x, compass.m.y);
    Serial.println(compassReport);
    compassReadings++;
  }
  else if(!isCalibrationCompleted) {
    Serial.println();
    Serial.println();
    setAllStop();
    // wait a second to signal shift in operating environment
    delay(1000);
    operatingEnvironment++;
    compassReadings = 0;
    setMotorOperatingEnvironment();
  } 
  else {
    // just to prevent needless looping ;-)
    delay(10000);
  }
  delay(50);

}

void setMotorOperatingEnvironment() {
  if(operatingEnvironment == 0) 
  {
    setOperatingEnvironmentZero();
  } 
  else if(operatingEnvironment == 1) 
  {
    setOperatingEnvironmentOne();
  } 
  else if(operatingEnvironment == 2) 
  {
    setOperatingEnvironmentTwo();
  } 
  else if(operatingEnvironment == 3) 
  {
    setOperatingEnvironmentThree();
  } 
  else if(operatingEnvironment == 4) 
  {
    setOperatingEnvironmentFour();
  } 
  else if(operatingEnvironment == 5) {
    isCalibrationCompleted = true;
    setAllStop();
    tone(buzzerDigitalPin, 500);
    delay(500);
    noTone(buzzerDigitalPin);
    delay(500);
    tone(buzzerDigitalPin, 500);
    delay(500);
    noTone(buzzerDigitalPin);
    delay(500);

  } 
  Serial.println("-------------------------------------------------------");
}  
// OPERATING ENVIRONMENTS -- only activate one -----------------------------------------------------------------

void setOperatingEnvironmentZero() {
  Serial.println("Setting Operating Environment Zero: [Right motor forward 50, Left motor reverse 50]");
  // Right motor forward 50, Left motor reverse 50
  digitalWrite(rightDirectionPin, FORWARD);
  digitalWrite(leftDirectionPin, REVERSE);
  analogWrite(rightSpeedPWMPin, 50);
  analogWrite(leftSpeedPWMPin, 50);
}

void setOperatingEnvironmentOne() {
  Serial.println("Setting Operating Environment One: [Right motor reverse 50, Left motor forward 50]");
  // Right motor reverse 50, Left motor forward 50
  digitalWrite(rightDirectionPin, REVERSE);
  digitalWrite(leftDirectionPin, FORWARD);
  analogWrite(rightSpeedPWMPin, 50);
  analogWrite(leftSpeedPWMPin, 50);
}

void setOperatingEnvironmentTwo() {
  Serial.println("Setting Operating Environment Two: [Right motor forward 100, Left motor forward 100]");
  // Right motor forward 100, Left motor forward 100
  digitalWrite(rightDirectionPin, FORWARD);
  digitalWrite(leftDirectionPin, FORWARD);
  analogWrite(rightSpeedPWMPin, 100);
  analogWrite(leftSpeedPWMPin, 100);
}

void setOperatingEnvironmentThree() {
  Serial.println("Setting Operating Environment Three: [Right motor reverse 100, Left motor reverse 100]");
  // Right motor reverse 100, Left motor reverse 100
  digitalWrite(rightDirectionPin, REVERSE);
  digitalWrite(leftDirectionPin, REVERSE);
  analogWrite(rightSpeedPWMPin, 100);
  analogWrite(leftSpeedPWMPin, 100);
}

void setOperatingEnvironmentFour() {
  Serial.println("Setting Operating Environment Four: [Right motor forward 0, Left motor forward 0]");
  // Right motor forward 0, Left motor forward 0
  digitalWrite(rightDirectionPin, FORWARD);
  digitalWrite(leftDirectionPin, FORWARD);
  analogWrite(rightSpeedPWMPin, 0);
  analogWrite(leftSpeedPWMPin, 0);
}

void setAllStop() {
  Serial.println("Setting All Stop: [Right motor forward 0, Left motor forward 0]");
  // Right motor forward 0, Left motor forward 0
  digitalWrite(rightDirectionPin, FORWARD);
  digitalWrite(leftDirectionPin, FORWARD);
  analogWrite(rightSpeedPWMPin, 0);
  analogWrite(leftSpeedPWMPin, 0);
}




