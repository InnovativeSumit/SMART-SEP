#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <TinyGPS++.h>

// ================= WIFI =================
const char* ssid = "Virus Activated";
const char* password = "9062589638";

String serverName = "https://script.google.com/macros/s/AKfycbxFpwQsHeIK-y7qaP07qpte_DSX2YC8PXR7FmmCKntpannXbq4qbOorDgpeK4FZf8jn/exec";

// ================= DHT =================
#define DHTPIN 26
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ================= ULTRASONIC =================
const int trigBio = 13;
const int echoBio = 33;
const int trigNonBio = 14;
const int echoNonBio = 27;

// ================= MQ4 =================
const int mq4Pin = 25;

// ================= GPS =================
TinyGPSPlus gps;
HardwareSerial gpsSerial(2);   // Serial2
#define RXD2 16
#define TXD2 17

// ================= TIMER =================
unsigned long lastSend = 0;
const long interval = 10000;

void setup() {

  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);

  pinMode(trigBio, OUTPUT);
  pinMode(echoBio, INPUT);
  pinMode(trigNonBio, OUTPUT);
  pinMode(echoNonBio, INPUT);

  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
}

void loop() {

  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (millis() - lastSend >= interval) {

    lastSend = millis();

    float bio = getLevel(trigBio, echoBio);
    float nonbio = getLevel(trigNonBio, echoNonBio);
    float temp = dht.readTemperature();
    float humid = dht.readHumidity();
    float methane = readMethane();

    double latitude = gps.location.isValid() ? gps.location.lat() : 0.0;
    double longitude = gps.location.isValid() ? gps.location.lng() : 0.0;

    Serial.println("\n==============================");
    Serial.println("SMART BIO-WASTE + GPS REPORT");
    Serial.println("==============================");

    Serial.printf("Temperature              : %.2f °C\n", temp);
    Serial.printf("Humidity                 : %.2f %%\n", humid);
    Serial.printf("Methane (MQ4)            : %.2f PPM\n", methane);
    Serial.printf("Biodegradable Waste      : %.2f %%\n", bio);
    Serial.printf("Non-Biodegradable Waste  : %.2f %%\n", nonbio);
    Serial.printf("Latitude                 : %.6f\n", latitude);
    Serial.printf("Longitude                : %.6f\n", longitude);

    if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;

      String url = serverName +
                   "?temp=" + String(temp,2) +
                   "&humid=" + String(humid,2) +
                   "&methane=" + String(methane,2) +
                   "&bio=" + String(bio,2) +
                   "&nonbio=" + String(nonbio,2) +
                   "&lat=" + String(latitude,6) +
                   "&lng=" + String(longitude,6);

      http.begin(url);
      int code = http.GET();
      Serial.print("Google Sheet Response: ");
      Serial.println(code);
      http.end();
    }
  }
}

// float getLevel(int trig, int echo) { 
//   digitalWrite(trig, LOW); 
//   delayMicroseconds(2); 
//   digitalWrite(trig, HIGH); 
//   delayMicroseconds(10); 
//   digitalWrite(trig, LOW); 
//   long duration = pulseIn(echo, HIGH, 30000); 
//   float distance = duration * 0.034 / 2; 
//   float level = map(distance, 0, 4, 100, 0); 
//   return constrain(level, 0, 100); }

float getLevel(int trig, int echo) { 
  digitalWrite(trig, LOW); 
  delayMicroseconds(5); 
  digitalWrite(trig, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(trig, LOW); 
  long duration = pulseIn(echo, HIGH, 25000); 
  // No echo → EMPTY
  if (duration == 0) return 0;
  float distance = duration * 0.034 / 2; 
  // Beyond bin height → EMPTY
  if (distance >= 5) return 0;
  // Clamp very small values for stability
  if (distance < 1.5) distance = 1.5;
  // 1.5–5 cm → 100–0 %
  float level = map(distance, 1.5, 5, 100, 0); 
  return constrain(level, 0, 100); 
}

// float getLevel(int trig, int echo) { 
//   digitalWrite(trig, LOW); 
//   delayMicroseconds(5); 
//   digitalWrite(trig, HIGH); 
//   delayMicroseconds(10); 
//   digitalWrite(trig, LOW); 

//   long duration = pulseIn(echo, HIGH, 25000); 

//   // No echo → EMPTY
//   if (duration == 0) return 0;

//   float distance = duration * 0.034 / 2; 

//   // Beyond bin height → EMPTY
//   if (distance >= 5) return 0;

//   // Clamp minimum distance
//   if (distance < 0) distance = 0;

//   // Map 0–5 cm → 100–10 %
//   float level = map(distance, 0, 3, 100, 10); 

//   return constrain(level, 10, 100); 
// }

float readMethane() {
  int value = analogRead(mq4Pin);
  float voltage = value * (3.3 / 4095.0);
  float ppm = voltage * 5;
  return constrain(ppm, 0, 5);
}