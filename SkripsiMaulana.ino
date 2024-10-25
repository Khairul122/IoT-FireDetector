#define BLYNK_TEMPLATE_ID "TMPL6wwzOefKr"
#define BLYNK_TEMPLATE_NAME "SKRIPSI"
#define BLYNK_AUTH_TOKEN "VZ14MQXb8noPchk-Xvl0kBSjZDiXsj0x"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* ssid = "Arul";
const char* pass = "123456789";

constexpr uint8_t MQ02_PIN = 35, MQ05_PIN = 34, SDA_PIN = 25, SCL_PIN = 26, BUZZER_PIN = 32, KIPAS_PIN = 33;
constexpr uint16_t SENSOR_MAX_MQ02 = 4095, SENSOR_MIN_MQ02 = 500, SENSOR_MIN_MQ05 = 0, SENSOR_MAX_MQ05 = 4095;
constexpr uint8_t GAS_THRESHOLD_MQ02 = 50;
constexpr uint16_t GAS_THRESHOLD_MQ05 = 2000;

LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

unsigned long warningStartTime = 0;
bool isWarningActive = false, kipasManualControl = false, buzzerManualControl = false;

inline int mapSensorValue(int value, int fromLow, int fromHigh, int toLow, int toHigh) {
  return map(constrain(value, fromLow, fromHigh), fromHigh, fromLow, toLow, toHigh);
}

void sendSensorData() {
  int rawMQ02 = analogRead(MQ02_PIN);
  int rawMQ05 = analogRead(MQ05_PIN);
  int mappedMQ02 = mapSensorValue(rawMQ02, SENSOR_MIN_MQ02, SENSOR_MAX_MQ02, 0, 100);
  
  bool isHighMQ02 = mappedMQ02 >= GAS_THRESHOLD_MQ02;
  bool isHighMQ05 = rawMQ05 < GAS_THRESHOLD_MQ05;
  
  Blynk.virtualWrite(V0, isHighMQ05);
  Blynk.virtualWrite(V1, isHighMQ02);
  
  if ((isHighMQ02 || isHighMQ05) && !isWarningActive) {
    Blynk.virtualWrite(V2, "Peringatan Kebakaran");
    Blynk.logEvent("alert", "PERINGATAN KEBAKARAN");
    isWarningActive = true;
    warningStartTime = millis();
  } else if (isWarningActive && (millis() - warningStartTime >= 5000)) {
    Blynk.virtualWrite(V2, "TIDAK ADA KEBAKARAN");
    isWarningActive = false;
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAS : ");
  lcd.print(isHighMQ05 ? "HIGH" : "LOW");
  lcd.setCursor(0, 1);
  lcd.print("ASAP: ");
  lcd.print(isHighMQ02 ? "HIGH" : "LOW");

  Serial.printf("MQ-02 Raw: %d, Mapped: %d%%, Status: %s | MQ-05 Raw: %d, Status: %s\n",
                rawMQ02, mappedMQ02, isHighMQ02 ? "HIGH" : "LOW", rawMQ05, isHighMQ05 ? "HIGH" : "LOW");

  if ((isHighMQ02 || isHighMQ05) && !kipasManualControl && !buzzerManualControl) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PERINGATAN!");
    lcd.setCursor(0, 1);
    lcd.print("KEBAKARAN");
    activateAlarm();
  }
}

BLYNK_WRITE(V3) {
  kipasManualControl = param.asInt();
  digitalWrite(KIPAS_PIN, kipasManualControl);
}

BLYNK_WRITE(V4) {
  buzzerManualControl = param.asInt();
  digitalWrite(BUZZER_PIN, buzzerManualControl);
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  
  pinMode(MQ02_PIN, INPUT);
  pinMode(MQ05_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(KIPAS_PIN, OUTPUT);

  lcd.clear();
  lcd.print("CONNECT WIFI");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  lcd.clear();
  lcd.print("WIFI DONE");
  delay(2000);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.virtualWrite(V2, "");

  timer.setInterval(1000L, sendSensorData);

  lcd.clear();
  lcd.print("Warming up...");
  delay(20000);
}

void activateAlarm() {
  if (!kipasManualControl) digitalWrite(KIPAS_PIN, HIGH);
  if (!buzzerManualControl) digitalWrite(BUZZER_PIN, HIGH);
  Blynk.logEvent("alert", "PERINGATAN KEBAKARAN");
  delay(5000);
  if (!kipasManualControl) digitalWrite(KIPAS_PIN, LOW);
  if (!buzzerManualControl) digitalWrite(BUZZER_PIN, LOW);
}

void loop() {
  Blynk.run();
  timer.run();
}