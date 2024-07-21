#include <Wire.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Adafruit_SHT4x.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "Adafruit_MAX1704X.h"
#include <Adafruit_VEML7700.h>
#include "Adafruit_seesaw.h"
#include <SPI.h>

// WiFi credentials
const char* ssid = "uluakihikihi";
const char* password = "pikaisgek";

// Define NTP client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 10 * 3600, 60000); // GMT+10 timezone

// Define TFT pins based on the provided pinout
#define TFT_CS 7
#define TFT_DC 39
#define TFT_RST 40
#define TFT_BACKLITE 45
#define TFT_I2C_POWER 21

// Initialize soil sensor
Adafruit_seesaw ss;

// Initialize battery monitor
Adafruit_MAX17048 maxlipo;

// Initialize SHT40 sensor
Adafruit_SHT4x sht4 = Adafruit_SHT4x();

// Initialize TFT screen
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Initialize VEML7700 light sensor
Adafruit_VEML7700 veml = Adafruit_VEML7700();

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10); // Wait until serial monitor opens

  Serial.println(F("\nPLANT SENSOR 3000"));

  // Connect to WiFi
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected.");

  // Initialize the time client
  timeClient.begin();

  // Initialize I2C
  Wire.begin();

  // Initialize the SHT40 sensor
  if (!sht4.begin()) {
    Serial.println("Couldn't find SHT40 sensor!");
    while (1) delay(10);
  }
  sht4.setPrecision(SHT4X_HIGH_PRECISION);
  sht4.setHeater(SHT4X_NO_HEATER);

  // Initialize the VEML7700 light sensor
  if (!veml.begin()) {
    Serial.println("Couldn't find VEML7700 sensor!");
    while (1) delay(10);
  }

  // Initialize the TFT backlight
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // Initialize the TFT I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // Initialize TFT display
  tft.init(135, 240); // Init ST7789 135x240
  tft.setRotation(2); // Rotate display by 180 degrees (landscape)
  tft.fillScreen(ST77XX_BLACK);

  // Initialize soil sensor
  if (!ss.begin(0x36)) {
    Serial.println("ERROR! seesaw not found");
    while (1) delay(1);
  } else {
    Serial.print("seesaw started! version: ");
    Serial.println(ss.getVersion(), HEX);
  }

  // Initialize battery monitor
  while (!maxlipo.begin()) {
    Serial.println(F("Couldn't find Adafruit MAX17048?\nMake sure a battery is plugged in!"));
    delay(2000);
  }

  Serial.print(F("Found MAX17048"));
  Serial.print(F(" with Chip ID: 0x"));
  Serial.println(maxlipo.getChipID(), HEX);

  // Set text properties
  tft.setTextSize(2);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
}

void loop() {
  // Update the time from NTP server
  timeClient.update();

  // Read data from SHT40
  sensors_event_t humidity, temp;
  sht4.getEvent(&humidity, &temp);

  // Get light level
  float lux = veml.readLux();

  // Get soil values
  uint16_t capread = ss.touchRead(0);
  Serial.print("Capacitive: ");
  Serial.println(capread);

  // Get battery voltage and percentage
  float cellVoltage = maxlipo.cellVoltage();
  if (isnan(cellVoltage)) {
    Serial.println("Failed to read cell voltage, check battery is connected!");
    delay(1000);
    return;
  }
  float battery = maxlipo.cellPercent();

  // Get current time
  String formattedTime = timeClient.getFormattedTime();

  // Clear and update dynamic text areas independently
  tft.fillRect(10, 10, 220, 40, ST77XX_BLACK); // Clear Time and Temp
  tft.setCursor(10, 10);
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.print(formattedTime);
  tft.setCursor(10, 30);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("Temp:");
  tft.setCursor(10, 50);
  tft.setTextColor(ST77XX_YELLOW, ST77XX_BLACK);
  tft.print(temp.temperature, 1);
  tft.print(" *C");

  tft.fillRect(10, 70, 220, 20, ST77XX_BLACK); // Clear Humidity
  tft.setCursor(10, 70);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("Humidity:");
  tft.setCursor(10, 90);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.print(humidity.relative_humidity, 1);
  tft.print(" %");

  tft.fillRect(10, 110, 220, 20, ST77XX_BLACK); // Clear Light
  tft.setCursor(10, 110);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("Light:");
  tft.setCursor(10, 130);
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.print(lux, 1);
  tft.print(" lx");

  tft.fillRect(10, 150, 220, 20, ST77XX_BLACK); // Clear Moisture
  tft.setCursor(10, 150);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("Moisture:");
  tft.setCursor(10, 170);
  tft.setTextColor(ST77XX_MAGENTA, ST77XX_BLACK);
  tft.print(capread);

  tft.fillRect(10, 190, 220, 20, ST77XX_BLACK); // Clear Battery
  tft.setCursor(10, 190);
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print("Battery:");
  tft.setCursor(10, 210);
  tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  tft.print(battery, 1);
  tft.print("% ");

  // Print data to serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity.relative_humidity);
  Serial.println(" %");
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" *C");
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  Serial.print(F("Batt Voltage: "));
  Serial.print(cellVoltage, 3);
  Serial.println(" V");
  Serial.print(F("Batt Percent: "));
  Serial.print(battery, 1);
  Serial.println(" %");
  Serial.println("Current time: " + formattedTime);
  Serial.print("Capacitive: ");
  Serial.println(capread);

  // Delay before next reading
  delay(1000);
}
