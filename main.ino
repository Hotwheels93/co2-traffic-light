// CO2 Measurment using ESP8266, SDC30 NDIR sensor, LED traffic light and SSD1306 0.96" OLED display
// Pinout traffic light: red->D6, yellow->D5, green->D7, display + sensor i2c -> D1 + D2
// Author: Martin Hocquel - 0verflow.de
// Version: 0.1 (13.12.2020)
// Thresholds: green (< 1000), yellow (>=1000<2000), red (>=2000)
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include "SparkFun_SCD30_Arduino_Library.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT  64

const int red =  D6;
const int yellow =  D5;
const int green =  D7;

SCD30 airSensor;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  // Init LED traffic light
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(115200);
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.setCursor(0, 11);
  display.println("Maren Hans");
  display.setCursor(135, 20);
  display.print("Init sensor...");
  display.display();
  if (airSensor.begin() == false)
    {
      Serial.println("Air sensor not detected. Please check wiring. Freezing...");
      while (1);
    }
  delay(3000);
}
void updateData() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setFont(&FreeSerif9pt7b);
  display.setCursor(0, 11);
  display.println("Temp:");
  display.setCursor(50, 12);
  display.setFont(&FreeSerifBold9pt7b);
  display.println(airSensor.getTemperature(),1);
  display.setFont(&FreeSerif9pt7b);
  display.setCursor(135, 20);
  display.print("CO2:");
  display.setCursor(55, 42);
  display.setFont(&FreeSerifBold9pt7b);
  display.setTextSize(2);
  display.print(airSensor.getCO2()); //CO2 value
  display.setFont(&FreeSerif9pt7b);
  display.setTextSize(1);
  display.setCursor(90, 58);
  display.print("ppm");
  display.display();
  if (airSensor.getCO2() < 1000) {
    digitalWrite(green, HIGH);
    digitalWrite(yellow, LOW);
    digitalWrite(red, LOW);
  }
  else if (airSensor.getCO2() < 2000) {
    digitalWrite(green, LOW);
    digitalWrite(yellow, HIGH);
    digitalWrite(red, LOW);
  }
  else { 
    digitalWrite(green, LOW);
    digitalWrite(yellow, LOW);
    digitalWrite(red, HIGH);
  }
}

void loop() {
if (airSensor.dataAvailable())
  {
    Serial.print("co2(ppm):");
    Serial.print(airSensor.getCO2());
    Serial.print(" temp(C):");
    Serial.print(airSensor.getTemperature(), 1);
    Serial.print(" humidity(%):");
    Serial.print(airSensor.getHumidity(), 1);
    Serial.println();;
    updateData();
  }
  else
    Serial.println("Requesting data");
  delay(500);
}
