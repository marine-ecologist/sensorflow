#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS3TRC.h>
#include <SPI.h>
#include <math.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// Define BLE Service and Characteristic UUIDs
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define PITCH_CHAR_UUID     "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define ROLL_CHAR_UUID      "c1c8f21d-2e89-4e17-8c39-23b5d72f935a"
#define YAW_CHAR_UUID       "e7a10bc2-5b7e-4e91-8b4b-7155b60ddc9a"

// BLE Variables
BLEServer* pServer = NULL;
BLECharacteristic* pPitchCharacteristic = NULL;
BLECharacteristic* pRollCharacteristic = NULL;
BLECharacteristic* pYawCharacteristic = NULL;
bool deviceConnected = false;

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// Use dedicated hardware SPI pins
#define TFT_CS     10
#define TFT_DC     9
#define TFT_RST    8
#define TFT_BACKLITE 12
#define TFT_I2C_POWER 11
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Create sensor objects
Adafruit_LIS3MDL lis3mdl; // magnetometer
Adafruit_LSM6DS3TRC lsm6ds3trc; // accelerometer / gyro

// Placeholder function to get battery percentage
int getBatteryPercentage() {
  // Return a dummy value for now
  return 75;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  // Initialize BLE
  BLEDevice::init("Tilt Sensor");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create BLE Characteristics
  pPitchCharacteristic = pService->createCharacteristic(PITCH_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pRollCharacteristic = pService->createCharacteristic(ROLL_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pYawCharacteristic = pService->createCharacteristic(YAW_CHAR_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  // Start the BLE Service
  pService->start();
  
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it on your phone!");

  // Initialize sensors
  while (!Serial) delay(10);
  Serial.println("Adafruit LIS3MDL & LSM6DS3TR-C test");
  
  // Initialize LIS3MDL sensor
  if (!lis3mdl.begin_I2C()) {
    Serial.println("Failed to find LIS3MDL chip");
    while (1) { delay(10); }
  }
  Serial.println("LIS3MDL Found!");

  // Initialize LSM6DS3TR-C sensor
  if (!lsm6ds3trc.begin_I2C()) {
    Serial.println("Failed to find LSM6DS3TR-C chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("LSM6DS3TR-C Found!");

  lis3mdl.setPerformanceMode(LIS3MDL_ULTRAHIGHMODE);
  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  lis3mdl.setDataRate(LIS3MDL_DATARATE_40_HZ);
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, true, false, true);

  lsm6ds3trc.configInt1(false, false, true);
  lsm6ds3trc.configInt2(false, true, false);

  // Initialize the TFT backlight
  pinMode(TFT_BACKLITE, OUTPUT);
  digitalWrite(TFT_BACKLITE, HIGH);

  // Initialize the TFT I2C power supply
  pinMode(TFT_I2C_POWER, OUTPUT);
  digitalWrite(TFT_I2C_POWER, HIGH);
  delay(10);

  // Initialize TFT display
  tft.init(135, 240); // Init ST7789 135x240
  tft.setRotation(3); // Adjust rotation if needed
  tft.fillScreen(ST77XX_BLACK);

  // Set text properties
  tft.setTextSize(2);

  // Print static label for heading
  tft.setCursor(20, 10);
  tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  tft.print("MC-01 Tiltmeter");
}

void loop() {
  // Read sensor data
  lis3mdl.read();
  sensors_event_t accel, gyro, temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

  // Calculate yaw in degrees (compass bearing)
  float yaw = atan2(lis3mdl.y, lis3mdl.x) * 180 / PI;
  if (yaw < 0) {
    yaw += 360;
  }

  // Calculate pitch and roll for accelerometer
  float pitch = atan2(accel.acceleration.y, sqrt(pow(accel.acceleration.x, 2) + pow(accel.acceleration.z, 2)));
  float roll = atan2(-accel.acceleration.x, accel.acceleration.z);

  // Convert pitch and roll to degrees
  pitch *= 180.0 / PI;
  roll *= 180.0 / PI;

  // Update BLE characteristics
  if (deviceConnected) {
    pPitchCharacteristic->setValue(pitch);
    pRollCharacteristic->setValue(roll);
    pYawCharacteristic->setValue(yaw);

    pPitchCharacteristic->notify();
    pRollCharacteristic->notify();
    pYawCharacteristic->notify();
  }

  // Update TFT display
  tft.setTextWrap(false);

  // Display pitch
  tft.setCursor(10, 40);
  tft.fillRect(0, 40, tft.width(), 18, ST77XX_BLACK);
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.print("Pitch: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(pitch, 2);

  // Display roll
  tft.setCursor(10, 60);
  tft.fillRect(0, 60, tft.width(), 18, ST77XX_BLACK);
  tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  tft.print("Roll: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(roll, 2);

  // Display yaw (bearing)
  tft.setCursor(10, 80);
  tft.fillRect(0, 80, tft.width(), 18, ST77XX_BLACK);
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.print("Yaw: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(yaw, 2);

  // Display temperature
  tft.setCursor(10, 100);
  tft.fillRect(0, 100, tft.width(), 18, ST77XX_BLACK);
  tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  tft.print("Temp: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(temp.temperature, 2);


  // Delay before next reading
  delay(200);
}
