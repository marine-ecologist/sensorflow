// Calibrating the LIS3MDL Magnetometer:
//	1.	Setup and Initialization:
//      	•	Connect the LIS3MDL to the Arduino using I2C.
//	      •	Initialize the magnetometer.
//	2.	Data Collection:
//	      •	Rotate the sensor in all directions to collect a wide range of readings.
//	      •	Record the maximum and minimum values for each axis.
//	3.	Calculate Offsets and Scaling Factors:
//    	  •	Determine the offset for each axis by averaging the maximum and minimum values.
//	      •	Calculate the scaling factors to ensure the range is consistent across all axes.
//	4.	Apply Calibration:
//	      •	Use the offsets and scaling factors to adjust the raw magnetometer readings.

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS3MDL.h>

// Create a LIS3MDL instance
Adafruit_LIS3MDL lis3mdl;

// Variables to store calibration values
float mag_min_x, mag_min_y, mag_min_z;
float mag_max_x, mag_max_y, mag_max_z;
float offset_x, offset_y, offset_z;
float scale_x, scale_y, scale_z;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10);  // Wait for the serial connection to be established
  }

  // Initialize the sensor
  if (!lis3mdl.begin_I2C()) {
    Serial.println("Failed to find LIS3MDL chip");
    while (1) { delay(10); }
  }
  Serial.println("LIS3MDL found!");

  // Set initial min and max values for calibration
  mag_min_x = mag_min_y = mag_min_z = 32767;
  mag_max_x = mag_max_y = mag_max_z = -32767;
}

void loop() {
  // Read the magnetometer values
  sensors_event_t event;
  lis3mdl.getEvent(&event);

  // Update min and max values for each axis
  if (event.magnetic.x < mag_min_x) mag_min_x = event.magnetic.x;
  if (event.magnetic.y < mag_min_y) mag_min_y = event.magnetic.y;
  if (event.magnetic.z < mag_min_z) mag_min_z = event.magnetic.z;

  if (event.magnetic.x > mag_max_x) mag_max_x = event.magnetic.x;
  if (event.magnetic.y > mag_max_y) mag_max_y = event.magnetic.y;
  if (event.magnetic.z > mag_max_z) mag_max_z = event.magnetic.z;

  // Print the min and max values for calibration
  Serial.print("Min X: "); Serial.print(mag_min_x);
  Serial.print("  Max X: "); Serial.print(mag_max_x);
  Serial.print("  Min Y: "); Serial.print(mag_min_y);
  Serial.print("  Max Y: "); Serial.print(mag_max_y);
  Serial.print("  Min Z: "); Serial.print(mag_min_z);
  Serial.print("  Max Z: "); Serial.println(mag_max_z);

  delay(100);
}

void calculateOffsetsAndScales() {
  // Calculate offsets
  offset_x = (mag_min_x + mag_max_x) / 2.0;
  offset_y = (mag_min_y + mag_max_y) / 2.0;
  offset_z = (mag_min_z + mag_max_z) / 2.0;

  // Calculate scales
  scale_x = (mag_max_x - mag_min_x) / 2.0;
  scale_y = (mag_max_y - mag_min_y) / 2.0;
  scale_z = (mag_max_z - mag_min_z) / 2.0;

  float avg_scale = (scale_x + scale_y + scale_z) / 3.0;
  scale_x = avg_scale / scale_x;
  scale_y = avg_scale / scale_y;
  scale_z = avg_scale / scale_z;
}

void applyCalibration(float &x, float &y, float &z) {
  // Apply offsets
  x -= offset_x;
  y -= offset_y;
  z -= offset_z;

  // Apply scaling factors
  x *= scale_x;
  y *= scale_y;
  z *= scale_z;
}
