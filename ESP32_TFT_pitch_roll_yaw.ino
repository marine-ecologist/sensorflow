/// Firmware 0.9002 24/6/24

#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <Adafruit_LIS3MDL.h>
#include <Adafruit_LSM6DS3TRC.h>
//#include <TFT_eSPI.h> 
#include <SPI.h>
#include <math.h>

// Use dedicated hardware SPI pins
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Create sensor objects
Adafruit_LIS3MDL lis3mdl; // magnetometer
Adafruit_LSM6DS3TRC lsm6ds3trc; // accelerometer / gyro

// Placeholder function to get battery percentage
int getBatteryPercentage() {
  // Return a dummy value for now
  return 75;
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

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
  Serial.print("Performance mode set to: ");
  switch (lis3mdl.getPerformanceMode()) {
    case LIS3MDL_LOWPOWERMODE: Serial.println("Low"); break;
    case LIS3MDL_MEDIUMMODE: Serial.println("Medium"); break;
    case LIS3MDL_HIGHMODE: Serial.println("High"); break;
    case LIS3MDL_ULTRAHIGHMODE: Serial.println("Ultra-High"); break;
  }

  lis3mdl.setOperationMode(LIS3MDL_CONTINUOUSMODE);
  Serial.print("Operation mode set to: ");
  switch (lis3mdl.getOperationMode()) {
    case LIS3MDL_CONTINUOUSMODE: Serial.println("Continuous"); break;
    case LIS3MDL_SINGLEMODE: Serial.println("Single mode"); break;
    case LIS3MDL_POWERDOWNMODE: Serial.println("Power-down"); break;
  }

  lis3mdl.setDataRate(LIS3MDL_DATARATE_0_625_HZ);
  Serial.print("Data rate set to: ");
  switch (lis3mdl.getDataRate()) {
    case LIS3MDL_DATARATE_0_625_HZ: Serial.println("0.625 Hz"); break;
    case LIS3MDL_DATARATE_1_25_HZ: Serial.println("1.25 Hz"); break;
    case LIS3MDL_DATARATE_2_5_HZ: Serial.println("2.5 Hz"); break;
    case LIS3MDL_DATARATE_5_HZ: Serial.println("5 Hz"); break;
    case LIS3MDL_DATARATE_10_HZ: Serial.println("10 Hz"); break;
    case LIS3MDL_DATARATE_20_HZ: Serial.println("20 Hz"); break;
    case LIS3MDL_DATARATE_40_HZ: Serial.println("40 Hz"); break;
    case LIS3MDL_DATARATE_80_HZ: Serial.println("80 Hz"); break;
    case LIS3MDL_DATARATE_155_HZ: Serial.println("155 Hz"); break;
    case LIS3MDL_DATARATE_300_HZ: Serial.println("300 Hz"); break;
    case LIS3MDL_DATARATE_560_HZ: Serial.println("560 Hz"); break;
    case LIS3MDL_DATARATE_1000_HZ: Serial.println("1000 Hz"); break;
  }
  
  lis3mdl.setRange(LIS3MDL_RANGE_4_GAUSS);
  Serial.print("Range set to: ");
  switch (lis3mdl.getRange()) {
    case LIS3MDL_RANGE_4_GAUSS: Serial.println("+-4 gauss"); break;
    case LIS3MDL_RANGE_8_GAUSS: Serial.println("+-8 gauss"); break;
    case LIS3MDL_RANGE_12_GAUSS: Serial.println("+-12 gauss"); break;
    case LIS3MDL_RANGE_16_GAUSS: Serial.println("+-16 gauss"); break;
  }

  lis3mdl.setIntThreshold(500);
  lis3mdl.configInterrupt(false, false, true, // enable z axis
                          true, // polarity
                          false, // don't latch
                          true); // enabled!

  // configure accelerometer
  Serial.print("Accelerometer range set to: ");
  switch (lsm6ds3trc.getAccelRange()) {
  case LSM6DS_ACCEL_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case LSM6DS_ACCEL_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case LSM6DS_ACCEL_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case LSM6DS_ACCEL_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }

  Serial.print("Gyro range set to: ");
  switch (lsm6ds3trc.getGyroRange()) {
  case LSM6DS_GYRO_RANGE_125_DPS:
    Serial.println("125 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_250_DPS:
    Serial.println("250 degrees/s");
    break;
  case  LSM6DS_GYRO_RANGE_500_DPS:
    Serial.println("500 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_1000_DPS:
    Serial.println("1000 degrees/s");
    break;
  case LSM6DS_GYRO_RANGE_2000_DPS:
    Serial.println("2000 degrees/s");
    break;
  case ISM330DHCX_GYRO_RANGE_4000_DPS:
    break; // unsupported range for the DS33
  }

  Serial.print("Accelerometer data rate set to: ");
  switch (lsm6ds3trc.getAccelDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  Serial.print("Gyro data rate set to: ");
  switch (lsm6ds3trc.getGyroDataRate()) {
  case LSM6DS_RATE_SHUTDOWN:
    Serial.println("0 Hz");
    break;
  case LSM6DS_RATE_12_5_HZ:
    Serial.println("12.5 Hz");
    break;
  case LSM6DS_RATE_26_HZ:
    Serial.println("26 Hz");
    break;
  case LSM6DS_RATE_52_HZ:
    Serial.println("52 Hz");
    break;
  case LSM6DS_RATE_104_HZ:
    Serial.println("104 Hz");
    break;
  case LSM6DS_RATE_208_HZ:
    Serial.println("208 Hz");
    break;
  case LSM6DS_RATE_416_HZ:
    Serial.println("416 Hz");
    break;
  case LSM6DS_RATE_833_HZ:
    Serial.println("833 Hz");
    break;
  case LSM6DS_RATE_1_66K_HZ:
    Serial.println("1.66 KHz");
    break;
  case LSM6DS_RATE_3_33K_HZ:
    Serial.println("3.33 KHz");
    break;
  case LSM6DS_RATE_6_66K_HZ:
    Serial.println("6.66 KHz");
    break;
  }

  lsm6ds3trc.configInt1(false, false, true); // accelerometer DRDY on INT1
  lsm6ds3trc.configInt2(false, true, false); // gyro DRDY on INT2

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
  
  // magnetometer get X Y and Z data at once
  lis3mdl.read(); 

  // get event for accelerometer
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  lsm6ds3trc.getEvent(&accel, &gyro, &temp);

  // Get battery percentage
  int battery = getBatteryPercentage();

  // Get current time
  unsigned long currentMillis = millis();
  int seconds = (currentMillis / 1000) % 60;
  int minutes = (currentMillis / (1000 * 60)) % 60;
  int hours = (currentMillis / (1000 * 60 * 60)) % 24;

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

  // Update TFT display
  tft.setTextWrap(false);

  // Display pitch
  tft.setCursor(10, 40);
  tft.fillRect(0, 40, tft.width(), 18, ST77XX_BLACK); // Clear previous text
  tft.setTextColor(ST77XX_CYAN, ST77XX_BLACK);
  tft.print("Pitch: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(pitch, 2);

  // Display roll
  tft.setCursor(10, 60);
  tft.fillRect(0, 60, tft.width(), 18, ST77XX_BLACK); // Clear previous text
  tft.setTextColor(ST77XX_BLUE, ST77XX_BLACK);
  tft.print("Roll: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(roll, 2);

  // Display yaw (bearing)
  tft.setCursor(10, 80);
  tft.fillRect(0, 80, tft.width(), 18, ST77XX_BLACK); // Clear previous text
  tft.setTextColor(ST77XX_GREEN, ST77XX_BLACK);
  tft.print("Yaw: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(yaw, 2);

  // Display temperature
  tft.setCursor(10, 100);
  tft.fillRect(0, 100, tft.width(), 18, ST77XX_BLACK); // Clear previous text
  tft.setTextColor(ST77XX_RED, ST77XX_BLACK);
  tft.print("Temp: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(temp.temperature, 2);

  // Display battery percentage
  tft.setCursor(10, 120);
  tft.fillRect(0, 120, tft.width(), 18, ST77XX_BLACK); // Clear previous text
  tft.setTextColor(ST77XX_ORANGE, ST77XX_BLACK);
  tft.print("Battery: ");
  tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
  tft.print(battery);
  tft.print("%");

  // Delay before next reading
  delay(200);
}
