#include <Servo.h>

#define VMOTOR   6
#define SERVO1_PIN 0
#define SERVO2_PIN 1
#define SERVO3_PIN 2

#define UV       7
#define UVS      8
#define PH       5
#define trigPin  18
#define echoPin  3
#define FLOW     19

#define FLOW_THRSH      1000
#define USER_DIST_CM    50
#define LEAVE_DIST_CM   50

Servo servo1;
Servo servo2;
Servo servo3;

long duration = 0;
int distance = 0;
int flow = 0;
int phValue = 0;
int proteinValue = 0;

unsigned long stateStartTime = 0;

enum State {
  WAIT_FOR_USER,
  WAIT_FOR_VOLUME,
  PH_MEASURE,
  PROTEIN_MEASURE,
  PROCESS_RESULT,
  WAIT_FOR_LEAVE,
  FLUSH_SYSTEM
};

State currentState = WAIT_FOR_USER;

// ---------- Helper functions ----------
int readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH, 30000);  // timeout 30 ms

  if (duration == 0) {
    return 999;  // no echo
  }

  return duration * 0.0343 / 2;
}

int readFlow() {
  return analogRead(FLOW);
}

int readPH() {
  return analogRead(PH);
}

int readProtein() {
  // place holder for protein sensor
  return analogRead(UVS);
}

void stopAllActuators() {
  digitalWrite(VMOTOR, LOW);
  digitalWrite(UV, LOW);
}

void startFlush() {
  digitalWrite(VMOTOR, HIGH);   
  servo1.write(90);             
  servo2.write(90);
  servo3.write(90);
}

void stopFlush() {
  digitalWrite(VMOTOR, LOW);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);
}

void processResult() {
  Serial.println("=== Processing Result ===");
  Serial.print("pH raw: ");
  Serial.println(phValue);

  Serial.print("Protein raw: ");
  Serial.println(proteinValue);

  // Placeholder decision logic
  if (phValue < 1000 || phValue > 3000 || proteinValue > 2000) {
    Serial.println("Possible abnormal result detected.");
    digitalWrite(UV, HIGH);   // example indicator
  } else {
    Serial.println("Result looks normal.");
    digitalWrite(UV, LOW);
  }
}

void setup() {
  Serial.begin(115200);

  servo1.attach(SERVO1_PIN);
  servo2.attach(SERVO2_PIN);
  servo3.attach(SERVO3_PIN);

  pinMode(UV, OUTPUT);
  pinMode(UVS, INPUT);
  pinMode(VMOTOR, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(PH, INPUT);
  pinMode(FLOW, INPUT);

  stopAllActuators();

  Serial.println("System initialized.");
}

void loop() {
  distance = readDistanceCM();
  flow = readFlow();

  switch (currentState) {
    case WAIT_FOR_USER:
      if (distance < USER_DIST_CM) {
        Serial.println("User detected.");
        currentState = WAIT_FOR_VOLUME;
      }
      break;

    case WAIT_FOR_VOLUME:
      Serial.print("Flow reading: ");
      Serial.println(flow);

      if (flow > FLOW_THRSH) {
        Serial.println("Flow threshold reached.");
        stateStartTime = millis();
        currentState = PH_MEASURE;
      }

      // user walked away before enough flow
      else if (distance > LEAVE_DIST_CM) {
        Serial.println("User left before threshold.");
        currentState = WAIT_FOR_USER;
      }
      break;

    case PH_MEASURE:
      // wait 5 seconds like your flowchart
      if (millis() - stateStartTime >= 5000) {
        phValue = readPH();
        Serial.print("pH measured: ");
        Serial.println(phValue);
        currentState = PROTEIN_MEASURE;
      }
      break;

    case PROTEIN_MEASURE:
      proteinValue = readProtein();
      Serial.print("Protein measured: ");
      Serial.println(proteinValue);
      currentState = PROCESS_RESULT;
      break;

    case PROCESS_RESULT:
      processResult();
      currentState = WAIT_FOR_LEAVE;
      break;

    case WAIT_FOR_LEAVE:
      if (distance > LEAVE_DIST_CM) {
        Serial.println("User left. Starting flush.");
        startFlush();
        stateStartTime = millis();
        currentState = FLUSH_SYSTEM;
      }
      break;

    case FLUSH_SYSTEM:
      // flush for 3 seconds
      if (millis() - stateStartTime >= 3000) {
        stopFlush();
        Serial.println("Flush complete.");
        currentState = WAIT_FOR_USER;
      }
      break;
  }

  delay(100);
}