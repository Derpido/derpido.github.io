// =============================================
// 4 Motor Test — ESP32 + 2x DRV8833
// ESP32 Arduino Core v3.x compatible
// Serial Monitor at 115200 baud
//
// M1 and M3 are physically mirrored so their
// speed is negated to match M2 and M4 direction
//
// Commands:
//   1 = Motor 1 forward (front-left)
//   2 = Motor 2 forward (front-right)
//   3 = Motor 3 forward (back-left)
//   4 = Motor 4 forward (back-right)
//   F = all motors forward
//   B = all motors backward
//   S = stop all
// =============================================

// --- Motor 1: Front-Left (DRV8833_A, OUT1+OUT2) ---
#define M1_IN1  25
#define M1_IN2  26

// --- Motor 2: Front-Right (DRV8833_A, OUT3+OUT4) ---
#define M2_IN1  27
#define M2_IN2  14

// --- Motor 3: Back-Left (DRV8833_B, OUT1+OUT2) ---
#define M3_IN1  12
#define M3_IN2  13

// --- Motor 4: Back-Right (DRV8833_B, OUT3+OUT4) ---
#define M4_IN1  32
#define M4_IN2  33

// Direction multipliers — flip if a motor spins wrong way
#define M1_DIR  -1
#define M2_DIR   1
#define M3_DIR  -1
#define M4_DIR   1

#define LEDC_FREQ   1000
#define LEDC_RES    8

void setMotor(int pin1, int pin2, int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    ledcWrite(pin1, speed);
    ledcWrite(pin2, 0);
  } else if (speed < 0) {
    ledcWrite(pin1, 0);
    ledcWrite(pin2, abs(speed));
  } else {
    ledcWrite(pin1, 0);
    ledcWrite(pin2, 0);
  }
}

void stopAll() {
  setMotor(M1_IN1, M1_IN2, 0);
  setMotor(M2_IN1, M2_IN2, 0);
  setMotor(M3_IN1, M3_IN2, 0);
  setMotor(M4_IN1, M4_IN2, 0);
}

void setup() {
  Serial.begin(115200);

  ledcAttach(M1_IN1, LEDC_FREQ, LEDC_RES);
  ledcAttach(M1_IN2, LEDC_FREQ, LEDC_RES);
  ledcAttach(M2_IN1, LEDC_FREQ, LEDC_RES);
  ledcAttach(M2_IN2, LEDC_FREQ, LEDC_RES);
  ledcAttach(M3_IN1, LEDC_FREQ, LEDC_RES);
  ledcAttach(M3_IN2, LEDC_FREQ, LEDC_RES);
  ledcAttach(M4_IN1, LEDC_FREQ, LEDC_RES);
  ledcAttach(M4_IN2, LEDC_FREQ, LEDC_RES);

  stopAll();

  Serial.println("4 Motor test ready.");
  Serial.println("1/2/3/4 = individual motors | F = all forward | B = all backward | S = stop");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    switch (cmd) {
      case '1':
        stopAll();
        setMotor(M1_IN1, M1_IN2, 200 * M1_DIR);
        Serial.println("Motor 1 (front-left) forward");
        break;
      case '2':
        stopAll();
        setMotor(M2_IN1, M2_IN2, 200 * M2_DIR);
        Serial.println("Motor 2 (front-right) forward");
        break;
      case '3':
        stopAll();
        setMotor(M3_IN1, M3_IN2, 200 * M3_DIR);
        Serial.println("Motor 3 (back-left) forward");
        break;
      case '4':
        stopAll();
        setMotor(M4_IN1, M4_IN2, 200 * M4_DIR);
        Serial.println("Motor 4 (back-right) forward");
        break;
      case 'F':
        setMotor(M1_IN1, M1_IN2, 200 * M1_DIR);
        setMotor(M2_IN1, M2_IN2, 200 * M2_DIR);
        setMotor(M3_IN1, M3_IN2, 200 * M3_DIR);
        setMotor(M4_IN1, M4_IN2, 200 * M4_DIR);
        Serial.println("All motors forward");
        break;
      case 'B':
        setMotor(M1_IN1, M1_IN2, -200 * M1_DIR);
        setMotor(M2_IN1, M2_IN2, -200 * M2_DIR);
        setMotor(M3_IN1, M3_IN2, -200 * M3_DIR);
        setMotor(M4_IN1, M4_IN2, -200 * M4_DIR);
        Serial.println("All motors backward");
        break;
      case 'S':
        stopAll();
        Serial.println("All stopped");
        break;
    }
  }
}
