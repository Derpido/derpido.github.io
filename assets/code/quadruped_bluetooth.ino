/*
  4-Servo Bluetooth Quadruped Walker
  -----------------------------------
  Hardware:
    - Arduino Uno + Sensor Shield V5
    - 4x hobby servos (one per leg) on pins 3, 5, 6, 9
    - HC-05 / HC-06 Bluetooth module:
        BT VCC -> 5V, GND -> GND
        BT TXD -> Arduino pin 10 (RX)
        BT RXD -> Arduino pin 11 (TX)  (use a voltage divider on this line
                  if your module isn't 5V-tolerant on RXD)

  Control:
    Send single characters over Bluetooth (e.g. from a "Bluetooth RC
    Controller" style Android app with directional buttons):
      F = forward   B = backward
      L = turn left R = turn right
      S = stop

  NOTE: The exact leg angles below (FWD_ANGLE / BACK_ANGLE / lift timing)
  depend on how your legs are physically mounted on the servo horns.
  Start with these values, then adjust STEP_DELAY and the angle constants
  below until the gait looks clean on your specific build.
*/

#include <Servo.h>
#include <SoftwareSerial.h>

// ---- Pin assignments ----
const int PIN_LEG_FL = 3;   // front-left
const int PIN_LEG_FR = 5;   // front-right
const int PIN_LEG_BL = 6;   // back-left
const int PIN_LEG_BR = 9;   // back-right

const int BT_RX = 10;       // to BT module TXD
const int BT_TX = 11;       // to BT module RXD

Servo legFL, legFR, legBL, legBR;
SoftwareSerial BTSerial(BT_RX, BT_TX);

// ---- Gait tuning ----
const int NEUTRAL_ANGLE = 90;   // resting/standing angle
const int FWD_ANGLE     = 120;  // leg swung forward
const int BACK_ANGLE    = 60;   // leg swung backward
const int STEP_DELAY    = 150;  // ms between each half-step; lower = faster

char command = 'S';

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);   // most HC-05/06 modules default to 9600

  legFL.attach(PIN_LEG_FL);
  legFR.attach(PIN_LEG_FR);
  legBL.attach(PIN_LEG_BL);
  legBR.attach(PIN_LEG_BR);

  stopRobot();
  Serial.println("Quadruped ready. Waiting for Bluetooth commands...");
}

void loop() {
  if (BTSerial.available()) {
    command = BTSerial.read();
    Serial.print("Command received: ");
    Serial.println(command);
  }

  switch (command) {
    case 'F': walkForward();  break;
    case 'B': walkBackward(); break;
    case 'L': turnLeft();     break;
    case 'R': turnRight();    break;
    case 'S': stopRobot();    break;
    default:  break; // ignore unrecognized bytes
  }
}

// ---- Gait functions ----
// Diagonal pairs (FL+BR, FR+BL) move together — a simple trot gait.

void walkForward() {
  // diagonal pair 1 swings forward, pair 2 swings backward
  legFL.write(FWD_ANGLE);
  legBR.write(FWD_ANGLE);
  legFR.write(BACK_ANGLE);
  legBL.write(BACK_ANGLE);
  delay(STEP_DELAY);

  // swap
  legFL.write(BACK_ANGLE);
  legBR.write(BACK_ANGLE);
  legFR.write(FWD_ANGLE);
  legBL.write(FWD_ANGLE);
  delay(STEP_DELAY);
}

void walkBackward() {
  legFL.write(BACK_ANGLE);
  legBR.write(BACK_ANGLE);
  legFR.write(FWD_ANGLE);
  legBL.write(FWD_ANGLE);
  delay(STEP_DELAY);

  legFL.write(FWD_ANGLE);
  legBR.write(FWD_ANGLE);
  legFR.write(BACK_ANGLE);
  legBL.write(BACK_ANGLE);
  delay(STEP_DELAY);
}

void turnLeft() {
  // right-side legs step forward, left-side legs step backward
  legFR.write(FWD_ANGLE);
  legBR.write(FWD_ANGLE);
  legFL.write(BACK_ANGLE);
  legBL.write(BACK_ANGLE);
  delay(STEP_DELAY);
  stopRobot();
}

void turnRight() {
  // left-side legs step forward, right-side legs step backward
  legFL.write(FWD_ANGLE);
  legBL.write(FWD_ANGLE);
  legFR.write(BACK_ANGLE);
  legBR.write(BACK_ANGLE);
  delay(STEP_DELAY);
  stopRobot();
}

void stopRobot() {
  legFL.write(NEUTRAL_ANGLE);
  legFR.write(NEUTRAL_ANGLE);
  legBL.write(NEUTRAL_ANGLE);
  legBR.write(NEUTRAL_ANGLE);
}
