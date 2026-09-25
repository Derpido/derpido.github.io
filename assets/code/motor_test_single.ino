// =============================================
// Single Motor Test — Motor 1 (Front-Left)
// DRV8833_A: IN1=GPIO25, IN2=GPIO26
// ESP32 Arduino Core v3.x compatible
// Send commands via Serial Monitor at 115200
// Commands: F = forward, B = backward, S = stop
// =============================================

#define IN1_PIN   25
#define IN2_PIN   26

#define LEDC_FREQ   1000    // 1kHz PWM
#define LEDC_RES    8       // 8-bit = 0 to 255

void setMotor(int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    ledcWrite(IN1_PIN, speed);   // forward
    ledcWrite(IN2_PIN, 0);
  } else if (speed < 0) {
    ledcWrite(IN1_PIN, 0);
    ledcWrite(IN2_PIN, abs(speed)); // reverse
  } else {
    ledcWrite(IN1_PIN, 0);       // stop
    ledcWrite(IN2_PIN, 0);
  }
}

void setup() {
  Serial.begin(115200);

  // New API: attach pin directly, no channels needed
  ledcAttach(IN1_PIN, LEDC_FREQ, LEDC_RES);
  ledcAttach(IN2_PIN, LEDC_FREQ, LEDC_RES);

  // Keep unused motor pins LOW so they don't float
pinMode(27, OUTPUT); digitalWrite(27, LOW);
pinMode(14, OUTPUT); digitalWrite(14, LOW);
pinMode(12, OUTPUT); digitalWrite(12, LOW);
pinMode(13, OUTPUT); digitalWrite(13, LOW);
pinMode(32, OUTPUT); digitalWrite(32, LOW);
pinMode(33, OUTPUT); digitalWrite(33, LOW);

  Serial.println("Motor test ready.");
  Serial.println("F = forward | B = backward | S = stop");
}

void loop() {
  if (Serial.available()) {
    char cmd = Serial.read();

    if (cmd == 'F') {
      setMotor(200);
      Serial.println("Running forward at 200/255");
    } else if (cmd == 'B') {
      setMotor(-200);
      Serial.println("Running backward at 200/255");
    } else if (cmd == 'S') {
      setMotor(0);
      Serial.println("Stopped");
    }
  }
}
