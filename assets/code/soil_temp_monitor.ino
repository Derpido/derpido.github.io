/*
  Soil Moisture + Temperature Monitor
  -------------------------------------
  Hardware:
    - Arduino Uno/Nano
    - Soil moisture sensor (analog output) -> A0
    - DHT11 temperature/humidity sensor -> pin 8
        (swap to DHT22 below if that's what you actually have)
    - 16x2 LCD (parallel, HD44780-compatible), wired as:
        RS -> 12   E -> 11
        D4 -> 5    D5 -> 4    D6 -> 3    D7 -> 2
        VO (contrast) -> potentiometer wiper, as usual

  Behavior:
    Reads soil moisture (%) and temperature (°C) every 2 seconds,
    displays both on the LCD, one per line.

  NOTE: Soil moisture sensors read HIGHER raw values when DRY and
  LOWER when WET (inverse). DRY_VALUE / WET_VALUE below need to be
  calibrated to your specific sensor:
    1. Upload this code, open Serial Monitor.
    2. With the probe fully dry (in air), note the raw ADC value printed.
    3. With the probe fully submerged in water, note that raw value.
    4. Plug both into DRY_VALUE and WET_VALUE below.
*/

#include <LiquidCrystal.h>
#include <DHT.h>

// ---- LCD ----
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// ---- Soil moisture ----
const int SOIL_PIN = A0;
int DRY_VALUE = 620;   // raw ADC reading in dry air -- CALIBRATE THIS
int WET_VALUE = 310;   // raw ADC reading fully in water -- CALIBRATE THIS

// ---- Temperature (DHT11) ----
#define DHTPIN 8
#define DHTTYPE DHT11   // change to DHT22 if that's your sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  dht.begin();

  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  delay(1500);
  lcd.clear();
}

void loop() {
  // ---- Soil moisture ----
  int rawSoil = analogRead(SOIL_PIN);
  int moisturePercent = map(rawSoil, DRY_VALUE, WET_VALUE, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100);

  // ---- Temperature ----
  float tempC = dht.readTemperature();

  // ---- Serial debug (use this to calibrate DRY_VALUE / WET_VALUE) ----
  Serial.print("Raw soil: ");
  Serial.print(rawSoil);
  Serial.print("  Moisture: ");
  Serial.print(moisturePercent);
  Serial.print("%  Temp: ");
  if (isnan(tempC)) {
    Serial.println("sensor error");
  } else {
    Serial.print(tempC);
    Serial.println(" C");
  }

  // ---- LCD display ----
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture ");
  lcd.print(moisturePercent);
  lcd.print("%  ");

  lcd.setCursor(0, 1);
  if (isnan(tempC)) {
    lcd.print("Temp: Error   ");
  } else {
    lcd.print("Temp: ");
    lcd.print(tempC, 1);
    lcd.print((char)223); // degree symbol
    lcd.print("C   ");
  }

  delay(2000);
}
