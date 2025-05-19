#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.setTextColor(WHITE);

  display.setTextSize(2); // Bigger text size

  // First Line: HELLO
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds("HELLO", 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 10);

  // Draw "HELLO" twice with a slight shift for bold effect
  display.print("HELLO");
  display.setCursor((SCREEN_WIDTH - w) / 2 + 1, 10); // shift slightly to right
  display.print("HELLO");

  // Second Line: SUMIT PAL
  display.getTextBounds("SUMIT PAL", 0, 0, &x1, &y1, &w, &h);
  display.setCursor((SCREEN_WIDTH - w) / 2, 40);

  // Draw "SUMIT PAL" twice with a slight shift for bold effect
  display.print("SUMIT PAL");
  display.setCursor((SCREEN_WIDTH - w) / 2 + 1, 40);
  display.print("SUMIT PAL");

  display.display();
}

void loop() {
  // Nothing here
}
