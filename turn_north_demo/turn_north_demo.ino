/*
 * Copyright (c) 2014 Daniel Fowler
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
 * associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all copies or substantial 
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN 
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "MovingAverage.cpp"
 
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
int neckServoPosition = 90;

// Motor forward or reverse
int FORWARD = LOW;
int REVERSE = HIGH;

// Serial debugging:
int baudrate = 9600;

// Digital Pins for driving RGB LED
int blueRgbLEDPin = 0;
int greenRgbLEDPin = 1;
int redRgbLEDPin = 2;

int movingAverageWindowSize = 31;

MovingAverage *xCoordinateMovingAverage;
MovingAverage *yCoordinateMovingAverage;

boolean isCalibrated = false;

float calibratedXCoord;
float calibratedYCoord;

float rejectionRegion = 30.5;

void setup() {
  
  setupMotors();

//  setupSerial();

  setupServo();

  setupBuzzer();

  setupCompass();
  
  setupRgbLED();

  analogReference(DEFAULT);
  
  xCoordinateMovingAverage = new MovingAverage(movingAverageWindowSize);
  yCoordinateMovingAverage = new MovingAverage(movingAverageWindowSize);
  isCalibrated = false;
  
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
  neckServo.detach();
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

void setupRgbLED() {
  pinMode(redRgbLEDPin, OUTPUT);
  pinMode(greenRgbLEDPin, OUTPUT);
  pinMode(blueRgbLEDPin, OUTPUT);
}

// LOOP ////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // calibrate
  if(isCalibrated) {
    // take measurement
    compass.read();
    
    // are we off?
    float xUpper = calibratedXCoord + rejectionRegion;
    float xLower = calibratedXCoord - rejectionRegion;
    
    float yUpper = calibratedYCoord + rejectionRegion;
    float yLower = calibratedYCoord - rejectionRegion;
    
    // TODO: rename
    bool isIn = compass.m.x < xUpper && compass.m.x > xLower && compass.m.y < yUpper && compass.m.y > xLower;
        
    if(isIn) {
      setLEDGreen();
      // don't move we're pointing the right way
      digitalWrite(rightDirectionPin, FORWARD);
      digitalWrite(leftDirectionPin, FORWARD);
      analogWrite(rightSpeedPWMPin, 0);
      analogWrite(leftSpeedPWMPin, 0);
    } else {
      setLEDRed();
      // we aren't pointing the right direction so turn till we are
      digitalWrite(rightDirectionPin, FORWARD);
      digitalWrite(leftDirectionPin, REVERSE);
      analogWrite(rightSpeedPWMPin, 50);
      analogWrite(leftSpeedPWMPin, 50);
    }
      
    
    // TODO: debug
//    Serial.print("X: ");
//    Serial.print(compass.m.x);
//    Serial.print(" Y: ");
//    Serial.print(compass.m.y);
//    Serial.print(" X Upper: ");
//    Serial.print(xUpper);
//    Serial.print(" X Lower: ");
//    Serial.print(xLower);
//    Serial.print(" Y Upper: ");
//    Serial.print(yUpper);
//    Serial.print(" Y Lower: ");
//    Serial.print(yLower);
//    Serial.print(" Is In: ");
//    Serial.println(isIn);
    
    
//    if(compass.m.x > (calibratedXCoord - rejectionRegion) && compass.m.x < (calibratedXCoord + rejectionRegion)
//        && compass.m.y > (calibratedYCoord - rejectionRegion) && compass.m.y < (calibratedYCoord + rejectionRegion)) {
//      // TODO: green led
//    } else {
//      // TODO: red led
//    }
  }
  else 
  {
    // TODO: turn on red led
    setLEDRed();
    
    // take measurement
    compass.read();
    
    // feed moving averages
    xCoordinateMovingAverage->add_value(compass.m.x);
    yCoordinateMovingAverage->add_value(compass.m.y);
    
    // check if window is full if so we are now calibrated, set green led
    if(xCoordinateMovingAverage->is_window_full() && xCoordinateMovingAverage->is_window_full()) {
      // TODO: turn on green LED
      setLEDGreen();
      
      // set calibrated values
      calibratedXCoord = xCoordinateMovingAverage->get_value();
      calibratedYCoord = yCoordinateMovingAverage->get_value();
      isCalibrated = true;
      
      // TODO: debug
//      Serial.print("X Coord: ");
//      Serial.print(calibratedXCoord);
//      Serial.print(" Y Coord: ");
//      Serial.println(calibratedYCoord);
      
      delay(5000);
    }
    delay(250);
    
  }
  // if calibrated take measurement and turn north
  
  
  
  
  // read compass
//  compass.read();  
//  int headingX = compass.m.x;
//  int headingY = compass.m.y;
//
//  if((headingX < northHeadingXMin || headingX > northHeadingXMax) 
//    && (headingY < northHeadingYMin || headingY > northHeadingYMax)) {
//    // turn motors
//    digitalWrite(rightDirectionPin, FORWARD);
//    digitalWrite(leftDirectionPin, REVERSE);
//    analogWrite(rightSpeedPWMPin, 50);
//    analogWrite(leftSpeedPWMPin, 50);
//    isBeeped = false;
//  } 
//  else {
//    // beep twice
//    digitalWrite(rightDirectionPin, FORWARD);
//    digitalWrite(leftDirectionPin, FORWARD);
//    analogWrite(rightSpeedPWMPin, 0);
//    analogWrite(leftSpeedPWMPin, 0);
//
//    if(!isBeeped) {
//      beepBuzzer();
//      beepBuzzer();
//      isBeeped = true;
//    }
//  }
}

void beepBuzzer() { 
  tone(buzzerDigitalPin, 300);
  delay(250);
  noTone(buzzerDigitalPin);
  delay(250);
}

void setLEDGreen() {
  showColor(true, false, true);
}

void setLEDRed() {
  showColor(false, true, true);
}

void showColor(boolean red, boolean green, boolean blue) {
  digitalWrite(redRgbLEDPin, red);
  digitalWrite(greenRgbLEDPin, green);
  digitalWrite(blueRgbLEDPin, blue);
  
}

