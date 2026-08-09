/*
  Dual-Mode Robot: Autonomous Obstacle Avoidance <-> Bluetooth Control
  ----------------------------------------------------------------------
  Hardware:
    - Arduino Uno/Nano
    - L298N motor driver + 2x gear motors
        ENA -> 5   IN1 -> 6   IN2 -> 7
        ENB -> 10  IN3 -> 8   IN4 -> 9
    - HC-SR04 ultrasonic sensor, mounted on a servo for 180-degree sweep
        TRIG -> 2   ECHO -> 3
        Servo signal -> 11
    - HC-05/06 Bluetooth module
        TXD -> 12 (Arduino RX)   RXD -> 13 (Arduino TX)
    - Mode switch (toggle/slide switch) -> pin 4
        HIGH = Autonomous obstacle-avoiding mode
        LOW  = Bluetooth manual control mode
    - Surveillance camera: separate standalone module (FPV cam / phone),
      not wired into the Arduino — runs independently.

  Bluetooth mode commands (single characters, e.g. from a
  "Bluetooth RC Controller" app):
      F = forward   B = backward
      L = left      R = right
      S = stop
*/

#include <Servo.h>
#include <SoftwareSerial.h>

// ---- Motor driver pins ----
const int ENA = 5, IN1 = 6, IN2 = 7;
const int ENB = 10, IN3 = 8, IN4 = 9;

// ---- Ultrasonic + servo ----
const int TRIG_PIN = 2, ECHO_PIN = 3;
const int SERVO_PIN = 11;
Servo scanServo;

// ---- Bluetooth ----
const int BT_RX = 12, BT_TX = 13;
SoftwareSerial BTSerial(BT_RX, BT_TX);

// ---- Mode switch ----
const int MODE_SWITCH_PIN = 4;

// ---- Tuning ----
const int MOTOR_SPEED = 180;        // 0-255
const int SAFE_DISTANCE_CM = 25;    // minimum clearance before turning
const int TURN_TIME_MS = 400;

void setup() {
  Serial.begin(9600);
  BTSerial.begin(9600);

  pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(MODE_SWITCH_PIN, INPUT_PULLUP); // switch to GND when active

  scanServo.attach(SERVO_PIN);
  scanServo.write(90); // center

  stopMotors();
}

void loop() {
  bool autonomousMode = digitalRead(MODE_SWITCH_PIN) == HIGH;

  if (autonomousMode) {
    runAutonomousMode();
  } else {
    runBluetoothMode();
  }
}

// =========================================================
// MODE 1: Autonomous obstacle avoidance
// =========================================================
void runAutonomousMode() {
  long distanceAhead = readDistanceCM();

  if (distanceAhead > 0 && distanceAhead < SAFE_DISTANCE_CM) {
    stopMotors();
    delay(150);

    // sweep and pick the clearest direction
    int bestAngle = scanForClearPath();

    if (bestAngle < 90) {
      turnRight();
    } else {
      turnLeft();
    }
    delay(TURN_TIME_MS);
    stopMotors();

  } else {
    driveForward();
  }
}

// Sweeps the servo from 0-180 degrees, returns the angle with
// the greatest measured clearance.
int scanForClearPath() {
  int bestAngle = 90;
  long bestDistance = -1;

  for (int angle = 0; angle <= 180; angle += 30) {
    scanServo.write(angle);
    delay(200); // let servo settle
    long d = readDistanceCM();
    if (d > bestDistance) {
      bestDistance = d;
      bestAngle = angle;
    }
  }
  scanServo.write(90); // recenter
  delay(150);
  return bestAngle;
}

// =========================================================
// MODE 2: Bluetooth manual control
// =========================================================
void runBluetoothMode() {
  if (BTSerial.available()) {
    char command = BTSerial.read();
    switch (command) {
      case 'F': driveForward();  break;
      case 'B': driveBackward(); break;
      case 'L': turnLeft();      break;
      case 'R': turnRight();     break;
      case 'S': stopMotors();    break;
      default: break;
    }
  }
}

// =========================================================
// Motor control
// =========================================================
void driveForward() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void driveBackward() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void turnLeft() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void turnRight() {
  analogWrite(ENA, MOTOR_SPEED);
  analogWrite(ENB, MOTOR_SPEED);
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
}

void stopMotors() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

// =========================================================
// Ultrasonic distance reading
// =========================================================
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return -1;

  return duration * 0.034 / 2;
}
