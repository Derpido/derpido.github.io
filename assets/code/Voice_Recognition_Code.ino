#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <driver/i2s.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ─── OLED Configuration ───────────────────────────
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ─── WiFi + Wit.ai ────────────────────────────────
const char* ssid                 = "YOUR_WIFI_SSID";
const char* password             = "YOUR_WIFI_PASSWORD";
const char* service_access_token = "YOUR_NEW_TOKEN_HERE";

// ─── I2S Pins ─────────────────────────────────────
#define I2S_WS      25
#define I2S_SD      33
#define I2S_SCK     26
#define BUTTON_PIN  4
#define SAMPLE_RATE 16000
#define I2S_PORT    I2S_NUM_0

// ─── Buffers ──────────────────────────────────────
#define BUFFER_SIZE 1024
int16_t buffer[BUFFER_SIZE];    // 16-bit buffer for sending to Wit.ai
int32_t buffer32[BUFFER_SIZE];  // 32-bit buffer for reading raw from mic

// ─── Display Helper ───────────────────────────────
void updateDisplay(String message) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(message);
  display.display();
}

// ─── I2S Setup ────────────────────────────────────
void setupI2S() {
  i2s_config_t config = {
    .mode                 = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate          = SAMPLE_RATE,
    .bits_per_sample      = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format       = I2S_CHANNEL_FMT_ONLY_RIGHT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags     = 0,
    .dma_buf_count        = 16,
    .dma_buf_len          = 512,
    .use_apll             = false
  };
  i2s_pin_config_t pin_config = {
    .bck_io_num   = I2S_SCK,
    .ws_io_num    = I2S_WS,
    .data_out_num = -1,
    .data_in_num  = I2S_SD
  };
  i2s_driver_install(I2S_PORT, &config, 0, NULL);
  i2s_set_pin(I2S_PORT, &pin_config);
}

// ─── Send Audio to Wit.ai ─────────────────────────
void sendAudioToWit() {
  WiFiClientSecure client;
  client.setInsecure();
  client.setTimeout(30000);

  if (!client.connect("api.wit.ai", 443)) {
    Serial.println("Connection failed");
    updateDisplay("Conn Failed");
    return;
  }
  Serial.println("Connected to Wit.ai");

  String header = "POST /speech?v=20230215 HTTP/1.1\r\n"
                  "Host: api.wit.ai\r\n"
                  "Authorization: Bearer " + String(service_access_token) + "\r\n"
                  "Content-Type: audio/raw;encoding=signed-integer;bits=16;rate=16000;endian=little\r\n"
                  "Transfer-Encoding: chunked\r\n"
                  "Connection: close\r\n\r\n";
  client.print(header);
  updateDisplay("Listening...");
  Serial.println("Listening...");

  size_t bytes_read;
  while (digitalRead(BUTTON_PIN) == LOW) {
    // Read 32-bit raw samples from mic
    i2s_read(I2S_PORT, buffer32, sizeof(buffer32), &bytes_read, portMAX_DELAY);

    if (bytes_read > 0) {
      // Convert 32-bit samples to 16-bit
      int samples = bytes_read / 4;  // 4 bytes per 32-bit sample
      for (int i = 0; i < samples; i++) {
        buffer[i] = (int16_t)(buffer32[i] >> 16);  // fixed: was 11, now 16
      }

      // Send 16-bit data to Wit.ai
      size_t send_bytes = samples * 2;  // 2 bytes per 16-bit sample
      client.printf("%X\r\n", send_bytes);
      client.write((uint8_t*)buffer, send_bytes);
      client.print("\r\n");
      yield();
    }
  }
  Serial.println("Button released — sending to Wit.ai");

  client.print("0\r\n\r\n");
  updateDisplay("Processing...");

  // ─── Response Reading with Two-Stage Timeout ────
  String finalResult = "";
  unsigned long lastDataTime = millis();
  bool responseStarted = false;

  while (client.connected() || client.available()) {
    unsigned long idleLimit = responseStarted ? 2000 : 10000;

    if (millis() - lastDataTime > idleLimit) {
      Serial.println("Timed out waiting for response");
      break;
    }

    if (client.available()) {
      lastDataTime = millis();
      responseStarted = true;
      String line = client.readStringUntil('\n');
      Serial.println("Response: " + line);

      int textIndex = line.indexOf("\"text\": \"");
      if (textIndex != -1) {
        int start = textIndex + 9;
        int end   = line.indexOf("\"", start);
        finalResult = line.substring(start, end);
        Serial.println("Extracted: " + finalResult);
      }
    }
    delay(1);
  }

  client.stop();
  if (finalResult != "") {
    updateDisplay(finalResult);
    Serial.println("Final: " + finalResult);
  } else {
    updateDisplay("No speech detected");
    Serial.println("No speech detected");
  }
}

// ─── Setup ────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  updateDisplay("Connecting...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.println("WiFi connected: " + WiFi.localIP().toString());

  setupI2S();
  updateDisplay("Ready.");
  Serial.println("Ready");
}

// ─── Loop ─────────────────────────────────────────
void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed");
    sendAudioToWit();
    while (digitalRead(BUTTON_PIN) == LOW) delay(10);
    delay(200);
  }
}