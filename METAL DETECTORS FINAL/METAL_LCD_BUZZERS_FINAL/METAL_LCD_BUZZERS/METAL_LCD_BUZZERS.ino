#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// ===== LCD Setup =====
LiquidCrystal_I2C lcd(0x27, 16, 2);  

// ===== Pin Setup =====
const int sensorPin = 4;
const int buzzerPin = 5;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  lcd.init();           
  lcd.backlight();      

  // Initializing message
  lcd.setCursor(1, 0);
  lcd.print("INITIALIZING");
  lcd.setCursor(2, 1);
  lcd.print("THE SYSTEM");
  delay(2000);

  lcd.clear();
  showHome();
}

void loop() {

  if (digitalRead(sensorPin) == LOW) {  // Change to HIGH if your sensor is PNP
    
    lcd.clear();

    // Centered Message
    lcd.setCursor(2, 0);  
    lcd.print("  METAL IS");

    lcd.setCursor(2, 1);  
    lcd.print("  DETECTED");

    // Buzzer beep 500 milliseconds
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);

    delay(2000);   // Display stays 2 seconds

    lcd.clear();
    showHome();
  }
}

// ===== Home Screen =====
void showHome() {
  lcd.setCursor(3, 0);  
  lcd.print("AI POWERED");

  lcd.setCursor(3, 1);  
  lcd.print("SMART SEP");
}