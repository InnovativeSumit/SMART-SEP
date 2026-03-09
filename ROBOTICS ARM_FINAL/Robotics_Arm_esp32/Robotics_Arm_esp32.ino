/*
  🌐 13 Servo WiFi Control – ESP32 DevKit V1
  WebSocket based control system

  Servos controlled via WiFi app or browser
*/

#include <WiFi.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>
#include <ESP32Servo.h>

/* ---------------- WIFI SETTINGS ---------------- */

const char* ssid = "Virus Activated";
const char* password = "9062589638";

const char* mdnsName = "esp32-servo";

/* ---------------- SERVO SETTINGS ---------------- */

#define NUM_SERVOS 13

const uint8_t servoPins[NUM_SERVOS] =
{
  4,   // Servo 1
  13,  // Servo 2
  14,  // Servo 3
  18,  // Servo 4
  19,  // Servo 5
  21,  // Servo 6
  22,  // Servo 7
  23,  // Servo 8
  25,  // Servo 9
  26,  // Servo 10
  27,  // Servo 11
  32,  // Servo 12
  33   // Servo 13
};

Servo servos[NUM_SERVOS];

/* ---------------- WEBSOCKET ---------------- */

WebSocketsServer webSocket = WebSocketsServer(80);

/* ---------------- SETUP ---------------- */

void setup()
{

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("ESP32 13 Servo WiFi Controller Starting...");

  /* ---- Attach Servos ---- */

  for (int i = 0; i < NUM_SERVOS; i++)
  {
    servos[i].setPeriodHertz(50);
    servos[i].attach(servoPins[i], 500, 2400);
    servos[i].write(90);
  }

  Serial.println("All servos initialized at 90°");

  /* ---- Connect WiFi ---- */

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("=================================");
  Serial.println("WiFi Connected Successfully");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.print("WebSocket URL: ws://");
  Serial.println(WiFi.localIP());

  Serial.println("=================================");

  /* ---- mDNS ---- */

  if (MDNS.begin(mdnsName))
  {
    Serial.print("mDNS Started: http://");
    Serial.print(mdnsName);
    Serial.println(".local");
  }

  /* ---- Start WebSocket ---- */

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("WebSocket Server Started (Port 80)");
}

/* ---------------- LOOP ---------------- */

void loop()
{
  webSocket.loop();

  /* ---- Auto reconnect WiFi ---- */

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("WiFi Lost! Reconnecting...");

    WiFi.disconnect();
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }

    Serial.println();
    Serial.println("Reconnected!");
    Serial.print("New IP: ");
    Serial.println(WiFi.localIP());
  }
}

/* ---------------- WEBSOCKET EVENTS ---------------- */

void webSocketEvent(uint8_t client, WStype_t type, uint8_t * payload, size_t length)
{

  switch(type)
  {

    case WStype_CONNECTED:
      Serial.printf("Client %u Connected\n", client);
      break;

    case WStype_DISCONNECTED:
      Serial.printf("Client %u Disconnected\n", client);
      break;

    case WStype_BIN:

      if (length == 2)
      {

        byte servoID = payload[0];
        byte angle = payload[1];

        if (angle > 180)
        {
          Serial.println("Invalid Angle");
          return;
        }

        /* ---- Move ALL Servos ---- */

        if (servoID == 255)
        {
          Serial.printf("Moving ALL Servos to %d\n", angle);

          for (int i = 0; i < NUM_SERVOS; i++)
          {
            servos[i].write(angle);
          }
        }

        /* ---- Move Individual Servo ---- */

        else if (servoID >= 1 && servoID <= NUM_SERVOS)
        {
          Serial.printf("Servo %d -> %d degrees\n", servoID, angle);

          servos[servoID - 1].write(angle);
        }

        else
        {
          Serial.println("Invalid Servo ID");
        }
      }

      break;

    default:
      break;
  }
}