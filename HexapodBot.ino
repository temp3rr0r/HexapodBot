/*
The MIT License (MIT) Copyright (c) 2015 Konstantinos Theodorakos
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVO_COUNT 16
#define INTER_SERVO_DELAY 50
#define INTRA_SERVO_DELAY 50

#define NUM_LEGS 6
#define NUM_SERVOS_PER_LEG 2
#define NUM_EPA_PER_SERVO 3

#define DEFAULT_SERVO_MIN_EPA 220
#define DEFAULT_SERVO_NEUTRAL_EPA 300
#define DEFAULT_SERVO_MAX_EPA 350

#define MIN_VALUE 0
#define NEUTRAL_VALUE 1
#define MAX_VALUE 2

#define FACTOR_1A -0
#define FACTOR_1B 0

#define VERBOSE 0

// 6 legs, 2 servos per leg, 3 EPA (Min, Neutral, Max) per servo
uint16_t hexapodEpa[NUM_LEGS][NUM_SERVOS_PER_LEG][NUM_EPA_PER_SERVO];

// the 3rd servo for each leg, for 4 only legs though
uint16_t hexapodSteadyLegs[4][NUM_EPA_PER_SERVO];

/**
* Setup the End Point Adjustment for all the legs, all the servos
*/
void setupEpa() {
  
  if (VERBOSE)    
    Serial.println("-- Setting up the EPA for all the legs, all the servos.");

  // Do set defaults first
  for (int i = 0; i < NUM_LEGS; i++) {
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++) {
      hexapodEpa[i][j][0] = DEFAULT_SERVO_MIN_EPA;  
      hexapodEpa[i][j][1] = DEFAULT_SERVO_NEUTRAL_EPA;
      hexapodEpa[i][j][2] = DEFAULT_SERVO_MAX_EPA;
    }
  }
  
  // Custom - per individual servo settings
  hexapodEpa[0][0][MIN_VALUE] = DEFAULT_SERVO_MIN_EPA - 40;  
  hexapodEpa[0][0][NEUTRAL_VALUE] = DEFAULT_SERVO_NEUTRAL_EPA - 40;
  hexapodEpa[0][0][MAX_VALUE] = DEFAULT_SERVO_NEUTRAL_EPA + 50;

  hexapodEpa[0][1][0] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA - 90;  
  hexapodEpa[0][1][1] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA - 90;
  hexapodEpa[0][1][2] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA + 10;

  hexapodEpa[1][0][0] = DEFAULT_SERVO_MIN_EPA - 20;  
  hexapodEpa[1][0][1] = DEFAULT_SERVO_NEUTRAL_EPA - 60;
  hexapodEpa[1][0][2] = DEFAULT_SERVO_NEUTRAL_EPA - 20;

  hexapodEpa[1][1][0] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA - 50;  
  hexapodEpa[1][1][1] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA - 50;
  hexapodEpa[1][1][2] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA + 50;
  
  hexapodEpa[2][0][0] = DEFAULT_SERVO_MIN_EPA - 30;  
  hexapodEpa[2][0][1] = DEFAULT_SERVO_NEUTRAL_EPA;
  hexapodEpa[2][0][2] = DEFAULT_SERVO_NEUTRAL_EPA + 40;

  hexapodEpa[2][1][0] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA - 120;  
  hexapodEpa[2][1][1] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA - 120;
  hexapodEpa[2][1][2] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA + 80;

  hexapodEpa[3][0][0] = DEFAULT_SERVO_NEUTRAL_EPA + 40;  
  hexapodEpa[3][0][1] = DEFAULT_SERVO_NEUTRAL_EPA - 30;
  hexapodEpa[3][0][2] = DEFAULT_SERVO_NEUTRAL_EPA - 110;

  hexapodEpa[3][1][0] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA + 20;  
  hexapodEpa[3][1][1] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA + 20;
  hexapodEpa[3][1][2] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA - 80;
  
  hexapodEpa[4][0][0] = DEFAULT_SERVO_NEUTRAL_EPA - 10;  
  hexapodEpa[4][0][1] = DEFAULT_SERVO_NEUTRAL_EPA - 40;
  hexapodEpa[4][0][2] = DEFAULT_SERVO_NEUTRAL_EPA - 70;

  hexapodEpa[4][1][0] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA;  
  hexapodEpa[4][1][1] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA;
  hexapodEpa[4][1][2] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA - 100;
  
  hexapodEpa[5][0][0] = DEFAULT_SERVO_NEUTRAL_EPA - 0;  
  hexapodEpa[5][0][1] = DEFAULT_SERVO_NEUTRAL_EPA - 110;
  hexapodEpa[5][0][2] = DEFAULT_SERVO_NEUTRAL_EPA - 150;

  hexapodEpa[5][1][0] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA + 20;  
  hexapodEpa[5][1][1] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA + 20;
  hexapodEpa[5][1][2] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA - 80;

  // Custom, set neutral values for steady legs
  hexapodSteadyLegs[0][MIN_VALUE] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA;
  hexapodSteadyLegs[0][NEUTRAL_VALUE] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA - 100;
  hexapodSteadyLegs[0][MAX_VALUE] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA;
  
  hexapodSteadyLegs[1][MIN_VALUE] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA;
  hexapodSteadyLegs[1][NEUTRAL_VALUE] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA - 110;
  hexapodSteadyLegs[1][MAX_VALUE] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA;
  
  hexapodSteadyLegs[2][MIN_VALUE] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA;
  hexapodSteadyLegs[2][NEUTRAL_VALUE] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA + 10;
  hexapodSteadyLegs[2][MAX_VALUE] = FACTOR_1B + DEFAULT_SERVO_NEUTRAL_EPA;
  
  hexapodSteadyLegs[3][MIN_VALUE] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA;
  hexapodSteadyLegs[3][NEUTRAL_VALUE] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA + 10;
  hexapodSteadyLegs[3][MAX_VALUE] = FACTOR_1A + DEFAULT_SERVO_NEUTRAL_EPA;
}

/**
* Set all servos to the neutral position
*/
void servosToNeutral() {
  int currentServoNum;
  
  if (VERBOSE)    
    Serial.println("-- Setting all servos to the neutral position.");
  
  for (int i = 0; i < NUM_LEGS; i++) {
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++) {
      currentServoNum = i * NUM_SERVOS_PER_LEG + j;      
      pwm.setPWM(currentServoNum, 0, hexapodEpa[i][j][1]);
    }
  }

  // Also set the steady legs to neutral     
  pwm.setPWM(12, 0, hexapodSteadyLegs[0][1]);
  pwm.setPWM(13, 0, hexapodSteadyLegs[1][1]);
  pwm.setPWM(14, 0, hexapodSteadyLegs[2][1]);
  pwm.setPWM(15, 0, hexapodSteadyLegs[3][1]);
}

/**
* Set all servos to the maximum position
*/
void servosToMaximum() {
  int currentServoNum;
  
  if (VERBOSE)    
    Serial.println("-- Setting all servos to the maximum position.");
  
  for (int i = 0; i < NUM_LEGS; i++) {
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++) {
      currentServoNum = i * NUM_SERVOS_PER_LEG + j;      
      pwm.setPWM(currentServoNum, 0, hexapodEpa[i][j][2]);
    }
  }
}

/**
* Set all servos to the minimum position
*/
void servosToMinimum() {
  int currentServoNum;
  
  if (VERBOSE)    
    Serial.println("-- Setting all servos to the minimum position.");
  
  for (int i = 0; i < NUM_LEGS; i++) {
    for (int j = 0; j < NUM_SERVOS_PER_LEG; j++) {
      currentServoNum = i * NUM_SERVOS_PER_LEG + j;      
      pwm.setPWM(currentServoNum, 0, hexapodEpa[i][j][0]);
    }
  }
}

/**
* Moves a servo from a pulse length to a desired pulse length.
* Also checks validity of input and automatically adjusts Increase or Decrease of the pulse length.
*/
void moveServo(int servoNum, int pulseLengthFrom, int pulseLengthTo) {
  int legNum;
  int legServoNum, pulseLengthMin, pulseLengthMax;
  bool commandValid;
  
  if (VERBOSE)    
    Serial.println("-- Moving a servo from a pulse length to a desired pulse length.");
  
  // Check command validity
  if (servoNum >= 0 && servoNum < NUM_SERVOS_PER_LEG * NUM_LEGS) {
    legNum = servoNum / NUM_SERVOS_PER_LEG;
    legServoNum = servoNum % NUM_SERVOS_PER_LEG;       
    
    if (legNum >= 0 && legNum < NUM_LEGS && legServoNum >= 0 && legServoNum < NUM_SERVOS_PER_LEG) {
      int pulseLengthMin, pulseLengthMax;
      pulseLengthMin = hexapodEpa[legNum][legServoNum][0];
      pulseLengthMax = hexapodEpa[legNum][legServoNum][2];
      
      if (pulseLengthFrom >= pulseLengthMin && pulseLengthFrom <= pulseLengthMax && pulseLengthTo >= pulseLengthMin && pulseLengthTo <= pulseLengthMax) {
        commandValid = true;
        if (VERBOSE) 
          Serial.println("-- Command was valid.");
      }
    }
  }
  
  if (commandValid) {
    if (VERBOSE) {
      Serial.print("-- Move leg ");
      Serial.print(legNum);     
      Serial.print("(servo number: ");   
      Serial.print(servoNum);     
      Serial.print("): from ");
      Serial.print(pulseLengthFrom);
      Serial.print(" to ");
      Serial.println(pulseLengthTo);
    }
    
    if (pulseLengthFrom < pulseLengthTo) {
      for (uint16_t pulseLength = pulseLengthFrom; pulseLength < pulseLengthTo; pulseLength++) {
        pwm.setPWM(servoNum, 0, pulseLength);
      }  
      delay(INTRA_SERVO_DELAY);
    } else if (pulseLengthFrom > pulseLengthTo) {
      for (uint16_t pulseLength = pulseLengthFrom; pulseLength > pulseLengthTo; pulseLength--) {
        pwm.setPWM(servoNum, 0, pulseLength);
      }  
      delay(INTRA_SERVO_DELAY);
    } else { // Do nothing, stays to the same position
    
      if (VERBOSE) 
        Serial.println("-- Move: Nothing");
    }
  }
  
}

void setup() {
  Serial.begin(9600);
  
  setupEpa();
    
  Serial.println("-- 16 channel Servo test!");
  pwm.begin();  
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  
  servosToNeutral();
}

// you can use this function if you'd like to set the pulse length in seconds
// e.g. setServoPulse(0, 0.001) is a ~1 millisecond pulse width. its not precise!
void setServoPulse(uint8_t n, double pulse) {
  double pulselength;
  
  pulselength = 1000000;   // 1,000,000 us per second
  pulselength /= 60;   // 60 Hz
  Serial.print(pulselength); Serial.println("--  us per period"); 
  pulselength /= 4096;  // 12 bits of resolution
  Serial.print(pulselength); Serial.println("--  us per bit"); 
  pulse *= 1000;
  pulse /= pulselength;
  Serial.println(pulse);
  pwm.setPWM(n, 0, pulse);
}

/**
* Used in forward(). Automates servo per leg movement, DRY principle
*/
void moveMultipleLegs(int currentLegServoNum, int oddEvenLeg, bool reverseMinMax) {
  int pulseLengthMin, pulseLengthMax, currentServoNum;
  
  if (VERBOSE)    
    Serial.println("-- Moving multiple legs.");
  
  for (int i = 0; i < NUM_LEGS; i++) { // For each leg
    currentServoNum = i * NUM_SERVOS_PER_LEG + currentLegServoNum;
    if (reverseMinMax) {
      pulseLengthMin = hexapodEpa[i][currentLegServoNum][2];
      pulseLengthMax = hexapodEpa[i][currentLegServoNum][0];
    } else {
      pulseLengthMin = hexapodEpa[i][currentLegServoNum][0];
      pulseLengthMax = hexapodEpa[i][currentLegServoNum][2];
    }
    
    if (i % 2 == oddEvenLeg) { // Odd leg: +
        moveServo(currentServoNum, pulseLengthMin, pulseLengthMax);      
    } else { // Even leg: -
        moveServo(currentServoNum, pulseLengthMax, pulseLengthMin);    
    }
    delay(INTER_SERVO_DELAY);
  }
}

void moveMultipleLegs(int currentLegServoNum, int oddEvenLeg, int pulseLengthMin, int pulseLengthMax) {
  int currentServoNum;
  
  if (VERBOSE)    
    Serial.println("-- Moving multiple legs.");
  
  for (int i = 0; i < NUM_LEGS; i++) { // For each leg
    currentServoNum = i * NUM_SERVOS_PER_LEG + currentLegServoNum;
    
    if (i % 2 == oddEvenLeg) { // Odd leg: +
        moveServo(currentServoNum, pulseLengthMin, pulseLengthMax);      
    } else { // Even leg: -
        moveServo(currentServoNum, pulseLengthMax, pulseLengthMin);    
    }
    delay(INTER_SERVO_DELAY);
  }
}

void phaseAPrimePlus(int legNum) {
  int currentServoNum;
  int currentLegServoNum = 1;
  currentServoNum = legNum * NUM_SERVOS_PER_LEG + currentLegServoNum;
  //moveServo(currentServoNum, hexapodEpa[legNum][currentLegServoNum][NEUTRAL_VALUE], hexapodEpa[legNum][currentLegServoNum][MAX_VALUE]);
  pwm.setPWM(currentServoNum, 0, hexapodEpa[legNum][currentLegServoNum][MAX_VALUE]);
}

void phaseBPrimePlus(int legNum) {
  int currentServoNum;
  int currentLegServoNum = 0;
  currentServoNum = legNum * NUM_SERVOS_PER_LEG + currentLegServoNum;
  //moveServo(currentServoNum, DEFAULT_SERVO_MAX_EPA, DEFAULT_SERVO_MIN_EPA);
  //moveServo(currentServoNum, hexapodEpa[legNum][currentLegServoNum][MAX_VALUE], hexapodEpa[legNum][currentLegServoNum][MIN_VALUE]);
  pwm.setPWM(currentServoNum, 0, hexapodEpa[legNum][currentLegServoNum][MIN_VALUE]);
}

void phaseCPrimePlus(int legNum) {
  int currentServoNum;
  int currentLegServoNum = 1;
  currentServoNum = legNum * NUM_SERVOS_PER_LEG + currentLegServoNum;  
 // moveServo(currentServoNum, DEFAULT_SERVO_MAX_EPA, DEFAULT_SERVO_MIN_EPA);
  //moveServo(currentServoNum, hexapodEpa[legNum][currentLegServoNum][MAX_VALUE], hexapodEpa[legNum][currentLegServoNum][MIN_VALUE]);
  pwm.setPWM(currentServoNum, 0, hexapodEpa[legNum][currentLegServoNum][MIN_VALUE]);
}

void phaseDPrimePlus(int legNum) {
  int currentServoNum;
  int currentLegServoNum = 0;
  currentServoNum = legNum * NUM_SERVOS_PER_LEG + currentLegServoNum;   
  //moveServo(currentServoNum, DEFAULT_SERVO_MIN_EPA, DEFAULT_SERVO_MAX_EPA);
  moveServo(currentServoNum, hexapodEpa[legNum][currentLegServoNum][MIN_VALUE], hexapodEpa[legNum][currentLegServoNum][MAX_VALUE]);
  pwm.setPWM(currentServoNum, 0, hexapodEpa[legNum][currentLegServoNum][MAX_VALUE]);
}

/**
* Moves the hexapod forward. Does it in 4 sub-phases (A to D)
*/
void forward() {
  if (VERBOSE)    
    Serial.println("-- Moving forward.");

//  phaseAPrimePlus(4);
  //delay(100);
  phaseBPrimePlus(4);
  delay(100);
  phaseCPrimePlus(4);
  delay(100);
  phaseAPrimePlus(0);
  delay(100);
  phaseBPrimePlus(0);
  delay(100);
  phaseCPrimePlus(0);
  delay(100);
  phaseAPrimePlus(2);
  delay(100);
  phaseBPrimePlus(2);
  delay(100);
  phaseCPrimePlus(2);
  delay(100);
  phaseAPrimePlus(1);
  phaseDPrimePlus(4);
  phaseDPrimePlus(0);
  phaseDPrimePlus(2);
  delay(100);

  
  //phaseAPrimePlus(1);
  //delay(100);
  phaseBPrimePlus(1);
  delay(100);
  phaseCPrimePlus(1);
  delay(100);
  phaseAPrimePlus(3);
  delay(100);
  phaseBPrimePlus(3);
  delay(100);
  phaseCPrimePlus(3);
  delay(100);
  phaseAPrimePlus(5);
  delay(100);
  phaseBPrimePlus(5);
  delay(100);
  phaseCPrimePlus(5);
  delay(100);
  phaseAPrimePlus(4);
  phaseDPrimePlus(1);
  phaseDPrimePlus(3);
  phaseDPrimePlus(5);
  delay(100);
 
}

void forwardPrime() {
  if (VERBOSE)    
    Serial.println("-- Moving forward.");
   
   // A
  // -> A1 - Front: Essentially, move A1 UP Neutral <-> Max
   if (VERBOSE)    
    Serial.println("-- Phase A.");
  for (int i = 0; i < NUM_LEGS; i++) { // For each leg
    if (i == 0 || i == 2 || i == 4) 
       phaseAPrimePlus(i);
    else if (i == 1 || i == 3 || i == 5)
       phaseCPrimePlus(i);
  }
  delay(2000);
  
    // B
  // -> A0 - Front
  if (VERBOSE)    
    Serial.println("-- Phase B.");
  for (int i = 0; i < NUM_LEGS; i++) { // For each leg
    if (i == 0 || i == 2 || i == 4) 
       phaseBPrimePlus(i);
    else if (i == 1 || i == 3 || i == 5)
       phaseDPrimePlus(i);
  }
  delay(2000);

    // C
  // -> A1 - Neutral - Neutral <-> Max
  if (VERBOSE)    
    Serial.println("-- Phase C.");
  for (int i = 0; i < NUM_LEGS; i++) { // For each leg
    if (i == 0 || i == 2 || i == 4) 
       phaseCPrimePlus(i);
    else if (i == 1 || i == 3 || i == 5)
       phaseAPrimePlus(i);
  }
  delay(2000);
  
  // D   
  // -> A0 - Neutral 
  if (VERBOSE)    
    Serial.println("-- Phase D.");
  for (int i = 0; i < NUM_LEGS; i++) { // For each leg
    if (i == 0 || i == 2 || i == 4)  
       phaseDPrimePlus(i);
    else if (i == 1 || i == 3 || i == 5)
       phaseBPrimePlus(i);
  }
  delay(2000);
}

void loop() {
  servosToNeutral(); delay(6000);
//  servosToMinimum();
//  delay(6000);
//  servosToMaximum();
//  delay(6000);
   
//  forward();  
}
