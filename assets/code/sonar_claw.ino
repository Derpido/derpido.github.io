/*
  Sonar-Triggered Claw Grabber
  -----------------------------
  Hardware:
    - Arduino (Nano/Uno)
    - HC-SR04 ultrasonic sensor, facing straight down
        VCC -> 5V, GND -> GND
        TRIG -> pin 9
        ECHO -> pin 10
    - 1x servo driving both claw fingers (via linkage) -> pin 6

  Behavior:
    1. Continuously measure distance to the floor/bin below.
    2. When distance drops under DETECT_DISTANCE_CM (claw has been
       lowered close enough to the floor), close the claw.
    3. Hold closed for HOLD_TIME_MS (7 seconds).
    4. Rotate the servo the opposite direction to open the claw.
    5. Wait until the claw is raised back up (distance goes large again)
       before it's allowed to trigger again — prevents it from
       immediately re-closing while still sitting near the floor.

  NOTE: OPEN_ANGLE / CLOSE_ANGLE depend on how your linkage is mounted.
  Test with the claw raised in the air first and adjust these two
  values until the fingers open/close fully without straining the servo.
*/

#include <Servo.h>

// ---- Pins ----
const int TRIG_PIN  = 9;
const int ECHO_PIN  = 10;
const int SERVO_PIN = 6;

// ---- Tuning ----
const int DETECT_DISTANCE_CM = 5;    // trigger threshold — how close to the floor before grabbing
const int RESET_DISTANCE_CM  = 15;   // must rise above this before it can trigger again
const unsigned long HOLD_TIME_MS = 7000; // how long to stay closed

const int OPEN_ANGLE  = 0;    // servo angle = claw fully open
const int CLOSE_ANGLE = 90;   // servo angle = claw fully closed

Servo clawServo;

enum ClawState { OPEN, CLOSED, WAITING_TO_RESET };
ClawState state = OPEN;

unsigned long closeStartTime = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  clawServo.attach(SERVO_PIN);
  clawServo.write(OPEN_ANGLE);
}

void loop() {
  long distance = readDistanceCM();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  switch (state) {

    case OPEN:
      if (distance > 0 && distance < DETECT_DISTANCE_CM) {
        closeClaw();
        closeStartTime = millis();
        state = CLOSED;
      }
      break;

    case CLOSED:
      if (millis() - closeStartTime >= HOLD_TIME_MS) {
        openClaw();
        state = WAITING_TO_RESET;
      }
      break;

    case WAITING_TO_RESET:
      // don't allow re-triggering until the claw has been lifted
      // well clear of the floor/bin
      if (distance > RESET_DISTANCE_CM) {
        state = OPEN;
      }
      break;
  }

  delay(100); // simple polling rate for the sonar
}

// ---- Claw control ----
void closeClaw() {
  clawServo.write(CLOSE_ANGLE);
  Serial.println("Claw CLOSED");
}

void openClaw() {
  clawServo.write(OPEN_ANGLE);
  Serial.println("Claw OPEN");
}

// ---- Sonar reading ----
// Returns distance in cm, or -1 if no echo received (out of range)
long readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout (~5m max range)
  if (duration == 0) return -1;

  long distanceCM = duration * 0.034 / 2; // speed of sound ~0.034 cm/us
  return distanceCM;
}
