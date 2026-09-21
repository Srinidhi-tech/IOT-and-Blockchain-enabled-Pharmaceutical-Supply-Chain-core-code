#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Pin Definitions
#define SS_PIN       27  
#define RST_PIN      13  
#define DHTPIN       32  
#define DHTTYPE      DHT22
#define BUZZER_PIN   26 
#define GREEN_LED    25  // Safe Indicator
#define RED_LED      33  // Breach Indicator

// Cold-Chain Temperature Limits (2°C - 8°C)
const float TEMP_MIN = 2.0;
const float TEMP_MAX = 8.0;

MFRC522 rfid(SS_PIN, RST_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  delay(500);

  // Initialize Actuators
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // Initialize Display & Sensors
  dht.begin();
  Wire.begin(21, 22);
  lcd.init();
  lcd.backlight();

  // Boot Screen
  lcd.setCursor(0, 0);
  lcd.print("ColdChain System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");

  SPI.begin(18, 19, 23, 27);
  rfid.PCD_Init();

  delay(2000);
  lcd.clear();
}

void loop() {
  float tempC = dht.readTemperature();

  if (isnan(tempC)) {
    tempC = 25.0; // Fallback
  }

  bool isBreached = (tempC < TEMP_MIN || tempC > TEMP_MAX);

  // Continuous System Status via LEDs
  if (isBreached) {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
  } else {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
  }

  // Scan RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Med: Insulin");

    lcd.setCursor(0, 1);
    lcd.print("T:" + String(tempC, 1) + "C ");

    if (isBreached) {
      lcd.print("[BREACH]");
      
      // Flash Red LED and Beep Buzzer
      for (int i = 0; i < 3; i++) {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(BUZZER_PIN, HIGH);
        delay(150);
        digitalWrite(RED_LED, LOW);
        digitalWrite(BUZZER_PIN, LOW);
        delay(100);
      }
      digitalWrite(RED_LED, HIGH); // Leave RED on after alert
    } else {
      lcd.print("[SAFE]");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(3000);
  } 
  else {
    // Idle Live Status
    lcd.setCursor(0, 0);
    lcd.print("Zone: Whse-Alpha ");
    lcd.setCursor(0, 1);
    lcd.print("Live T: " + String(tempC, 1) + " C   ");
    delay(300);
  }
}
