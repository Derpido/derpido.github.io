// =============================================
// Soccer Robot — PS5 DualSense Controller
// ESP32 + 2x DRV8833 + Mecanum Wheels
// Requires: Bluepad32 board package (v2.x core)
// Board: ESP32 + Bluepad32 Arduino > ESP32 Dev Module
//
// Controls:
//   Left stick  Y = forward/backward
//   Left stick  X = strafe left/right
//   Right stick X = rotate
//   Cross (X)     = toggle stop
// =============================================

#include <Bluepad32.h>

// --- Motor pins ---
#define M1_IN1  25
#define M1_IN2  26
#define M2_IN1  27
#define M2_IN2  14
#define M3_IN1  12
#define M3_IN2  13
#define M4_IN1  32
#define M4_IN2  33

// --- LEDC channels (one per pin, 0-7) ---
#define CH_M1_IN1  0
#define CH_M1_IN2  1
#define CH_M2_IN1  2
#define CH_M2_IN2  3
#define CH_M3_IN1  4
#define CH_M3_IN2  5
#define CH_M4_IN1  6
#define CH_M4_IN2  7

// Direction multipliers — left side motors are physically mirrored
#define M1_DIR  -1
#define M2_DIR   1
#define M3_DIR  -1
#define M4_DIR   1

#define LEDC_FREQ   1000
#define LEDC_RES    8
#define DEADZONE    20

GamepadPtr myGamepad = nullptr;

bool stopped = false;         // toggle stop state
bool prevCross = false;       // previous cross button state (for debounce)

// --------------------------------------------------
// Bluepad32 callbacks
// --------------------------------------------------
void onConnectedGamepad(GamepadPtr gp) {
  myGamepad = gp;
  Serial.println("Controller connected!");
}

void onDisconnectedGamepad(GamepadPtr gp) {
  myGamepad = nullptr;
  Serial.println("Controller disconnected");
}

// --------------------------------------------------
// setMotor: speed -255 to +255
// --------------------------------------------------
void setMotor(int ch1, int ch2, int speed) {
  speed = constrain(speed, -255, 255);

  if (speed > 0) {
    ledcWrite(ch1, speed);
    ledcWrite(ch2, 0);
  } else if (speed < 0) {
    ledcWrite(ch1, 0);
    ledcWrite(ch2, abs(speed));
  } else {
    ledcWrite(ch1, 0);
    ledcWrite(ch2, 0);
  }
}

void stopAll() {
  setMotor(CH_M1_IN1, CH_M1_IN2, 0);
  setMotor(CH_M2_IN1, CH_M2_IN2, 0);
  setMotor(CH_M3_IN1, CH_M3_IN2, 0);
  setMotor(CH_M4_IN1, CH_M4_IN2, 0);
}

// --------------------------------------------------
// drive: mecanum wheel mixing
// --------------------------------------------------
void drive(int vy, int vx, int omega) {
  int m1 = vy - vx + omega;  // front-left
  int m2 = vy + vx - omega;  // front-right
  int m3 = vy + vx + omega;  // back-left
  int m4 = vy - vx - omega;  // back-right

  // Scale down proportionally if any motor exceeds 255
  int maxVal = max(max(abs(m1), abs(m2)), max(abs(m3), abs(m4)));
  if (maxVal > 255) {
    m1 = m1 * 255 / maxVal;
    m2 = m2 * 255 / maxVal;
    m3 = m3 * 255 / maxVal;
    m4 = m4 * 255 / maxVal;
  }

  setMotor(CH_M1_IN1, CH_M1_IN2, m1 * M1_DIR);
  setMotor(CH_M2_IN1, CH_M2_IN2, m2 * M2_DIR);
  setMotor(CH_M3_IN1, CH_M3_IN2, m3 * M3_DIR);
  setMotor(CH_M4_IN1, CH_M4_IN2, m4 * M4_DIR);
}

int deadzone(int val) {
  return (abs(val) < DEADZONE) ? 0 : val;
}

void setup() {
  Serial.begin(115200);

  ledcSetup(CH_M1_IN1, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M1_IN1, CH_M1_IN1);
  ledcSetup(CH_M1_IN2, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M1_IN2, CH_M1_IN2);
  ledcSetup(CH_M2_IN1, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M2_IN1, CH_M2_IN1);
  ledcSetup(CH_M2_IN2, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M2_IN2, CH_M2_IN2);
  ledcSetup(CH_M3_IN1, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M3_IN1, CH_M3_IN1);
  ledcSetup(CH_M3_IN2, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M3_IN2, CH_M3_IN2);
  ledcSetup(CH_M4_IN1, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M4_IN1, CH_M4_IN1);
  ledcSetup(CH_M4_IN2, LEDC_FREQ, LEDC_RES);  ledcAttachPin(M4_IN2, CH_M4_IN2);

  stopAll();

  BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);

  Serial.println("Ready — hold PS + Create button to pair controller");
  Serial.println("Cross (X) = toggle stop");
}

void loop() {
  BP32.update();

  if (myGamepad && myGamepad->isConnected()) {

    // --- Stop button: Cross (X) toggles stop on/off ---
    bool crossNow = (myGamepad->buttons() & BUTTON_A);  // Cross = BUTTON_A in Bluepad32
    if (crossNow && !prevCross) {                        // detect fresh press only
      stopped = !stopped;
      Serial.println(stopped ? "STOPPED" : "RUNNING");
    }
    prevCross = crossNow;

    // --- If stopped, cut motors and skip drive ---
    if (stopped) {
      stopAll();
      return;
    }

    // --- Read sticks ---
    int lx =  myGamepad->axisX();
    int ly = -myGamepad->axisY();   // negated: stick up = forward
    int rx = -myGamepad->axisRX();  // negated: corrects spin direction

    int vx    = map(lx, -512, 511, -255, 255);
    int vy    = map(ly, -512, 511, -255, 255);
    int omega = map(rx, -512, 511, -255, 255);

    vx    = deadzone(vx);
    vy    = deadzone(vy);
    omega = deadzone(omega);

    drive(vy, vx, omega);

  } else {
    stopAll();
  }
}
