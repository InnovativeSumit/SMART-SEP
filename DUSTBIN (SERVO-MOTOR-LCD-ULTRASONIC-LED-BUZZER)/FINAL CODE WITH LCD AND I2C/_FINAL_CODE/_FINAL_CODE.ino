#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Wire.h>

Servo servo1;
const int trigPin = 10;
const int echoPin = 9;
const int blueLedPin = 6;  
const int greenLedPin = 5; 
const int buzzerPin = 7;   
long duration;
int distance = 0;
int potPin = A0;  
int soil = 0;
int fsoil = 0;

// Create LCD object with I2C address 0x27 (adjust if necessary)
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 16x2 LCD display with I2C interface



void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  pinMode(blueLedPin, OUTPUT); 
  pinMode(greenLedPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT); 
  servo1.attach(8);  
  
  // Initialize LCD
  lcd.begin(16, 2);  // 16 columns and 2 rows for the LCD
  lcd.backlight();
  
  // Display a startup message
  lcd.setCursor(0, 0);
  lcd.print("  INITIALIZNG");
  lcd.setCursor(0, 1);
  lcd.print("DETECTION SYSTEM");
  delay(2000); // Wait for 2 seconds
}

void loop() {
  distance = 0;
  fsoil = 0;

  // Measure distance using ultrasonic sensor
  for (int i = 0; i < 2; i++) {
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(7); 
    digitalWrite(trigPin, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(trigPin, LOW); 
    delayMicroseconds(10);
    duration = pulseIn(echoPin, HIGH); 
    distance += duration * 0.034 / 2;  
    delay(10);  
  }

  distance = distance / 2;  
  // Send properly formatted output for plotting
  Serial.print("Distance");  
  Serial.print("\t");       
  Serial.print(distance);    
  Serial.print("\t");        
  Serial.print("SoilMoisture"); 
  Serial.print("\t");        
  Serial.println(fsoil);     

  // Check if the distance is in range (between 1 cm and 15 cm)
  if (distance < 15 && distance > 1) {
    delay(1000);  
    // Measure soil moisture
    for (int i = 0; i < 3; i++) {  
      soil = analogRead(potPin);  
      soil = constrain(soil, 485, 1023);  
      fsoil += map(soil, 485, 1023, 100, 0);  
      delay(75);  
    }
    fsoil = fsoil / 3;  
    Serial.print("Soil Moisture: ");
    Serial.print(fsoil);
    Serial.println("%");

    // LCD output based on moisture detection
    lcd.clear();
    if (fsoil > 50) {
      Serial.print(" WET GARBAGE ");
      servo1.write(180);  
      digitalWrite(greenLedPin, HIGH);  
      digitalWrite(buzzerPin, HIGH);  
      delay(2000);  
      digitalWrite(greenLedPin, LOW);  
      digitalWrite(buzzerPin, LOW);  
      
      // Display on LCD
      lcd.setCursor(0, 0);
      lcd.print("   WET GARBAGE");
      lcd.setCursor(0, 1);
      lcd.print("  MOISTURE: ");
      lcd.print(fsoil);
      lcd.print("%");
      //delay(2000); // Display for 2 seconds
    } 
    else {
      Serial.print("DRY GARBAGE");
      servo1.write(0);  
      digitalWrite(blueLedPin, HIGH);  
      digitalWrite(buzzerPin, HIGH);  
      delay(2000);  
      digitalWrite(blueLedPin, LOW);  
      digitalWrite(buzzerPin, LOW);  
      
      // Display on LCD
      lcd.setCursor(0, 0);
      lcd.print("   DRY GARBAGE");
      lcd.setCursor(0, 1);
      lcd.print("  MOISTURE: ");
      lcd.print(fsoil);
      lcd.print("%");
      delay(2000); // Display for 2 seconds
    }
    
    // Reset servo to neutral position after actions
    servo1.write(90);

    // Add a 10-second delay before measuring distance again
    delay(10000);  // Wait for 10 seconds before measuring distance again
  } else {
    // If the distance is not in range, keep sensors relaxed
    // Serial.println("Distance is not in range (1-15 cm), Servo and sensors relaxed.");
    servo1.write(90);  
    digitalWrite(blueLedPin, LOW);  
    digitalWrite(greenLedPin, LOW); 
    digitalWrite(buzzerPin, LOW);  
    
    // Display default message
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("   DRY & WET ");
    lcd.setCursor(0, 1);
    lcd.print(" GARBAGE DETECT");
    delay(1000);  
  }
  
  distance = 0;
  fsoil = 0;
}
