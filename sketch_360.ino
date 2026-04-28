#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <HX711.h>

const int HX711_DT_PIN = 2;
const int HX711_SCK_PIN = 3; 
const int MQ2_PIN = A0;
const int RELAY_PIN = 7;
const int BUZZER_PIN = 8;

const int GAS_THRESHOLD = 600;       
const float LOW_GAS_WEIGHT =0.3;

float CALIBRATION_FACTOR = 90000.0; 

int RELAY_ON = HIGH; 
int RELAY_OFF = LOW;

LiquidCrystal_I2C lcd(0x27, 20, 4);
HX711 scale;

void setup() {
  Serial.begin(9600);
  
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  digitalWrite(RELAY_PIN, RELAY_OFF);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LPG Safety System");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  scale.begin(HX711_DT_PIN, HX711_SCK_PIN);
  scale.set_scale(CALIBRATION_FACTOR); 
  scale.tare();
}

void loop() {
  int gasLevel = analogRead(MQ2_PIN);
  float currentWeight = scale.get_units(5); 
  
  if(currentWeight < 0) {
    currentWeight = 0.0;
  }

  lcd.setCursor(0, 0);
  lcd.print("Weight: ");
  lcd.print(currentWeight, 2); 
  lcd.print(" kg   "); 

  lcd.setCursor(0, 1);
  lcd.print("Gas Lvl: ");
  lcd.print(gasLevel);
  lcd.print("    "); 

  if (gasLevel > GAS_THRESHOLD) {
    digitalWrite(RELAY_PIN, RELAY_ON); 
    digitalWrite(BUZZER_PIN, HIGH);    
    
    lcd.setCursor(0, 2);
    lcd.print("!! GAS LEAK !!      ");
    lcd.setCursor(0, 3);
    lcd.print("EVACUATE & FAN ON   ");
  } 
  else {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    digitalWrite(BUZZER_PIN, LOW);
    
    lcd.setCursor(0, 2);
    lcd.print("Status: Safe        ");
    lcd.setCursor(0, 3);
    lcd.print("                    ");


    if (currentWeight < LOW_GAS_WEIGHT && currentWeight > 0.05) {
      lcd.setCursor(0, 3);
      lcd.print("Warning: Low Gas!   ");
    }
  }

  delay(500); 
}