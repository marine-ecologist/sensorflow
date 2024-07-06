# sensorflow

Arduino code repository for flow sensor development (tilt meters) using an [Arduino Feather ESP32 S3](https://learn.adafruit.com/adafruit-esp32-s3-tft-feather/overview) and 9-axis IMU ([LIS3MDL](https://learn.adafruit.com/lis3mdl-triple-axis-magnetometer/overview)/[LSM6DS3TRC](https://learn.adafruit.com/adafruit-lsm6ds3tr-c-6-dof-accel-gyro-imu/overview)).

## Dev .ino files:
[ESP32_TFT_NE_bearing.ino](https://github.com/marine-ecologist/sensorflow/blob/main/ESP32_TFT_NE_bearing.ino)

<img src=https://github.com/marine-ecologist/sensorflow/assets/93563980/e8062c60-246c-48fd-962e-ce51cd6ec3f9 width=300>

[ESP32_TFT_pitch_roll_yaw.ino](https://github.com/marine-ecologist/sensorflow/blob/main/ESP32_TFT_pitch_roll_yaw.ino)

<img src=https://github.com/marine-ecologist/sensorflow/assets/93563980/12be611f-35cd-4336-8bef-010d3033674d width=300>

# Hardware notes

##  LIS3MDL (Magnetometer)

### Data rate:
	•	0.625 Hz: The sensor outputs data 0.625 times per second.
	•	1.25 Hz: The sensor outputs data 1.25 times per second.
	•	2.5 Hz: The sensor outputs data 2.5 times per second.
	•	5 Hz: The sensor outputs data 5 times per second.
	•	10 Hz: The sensor outputs data 10 times per second.
	•	20 Hz: The sensor outputs data 20 times per second.
	•	40 Hz: The sensor outputs data 40 times per second.
	•	80 Hz: The sensor outputs data 80 times per second.
	•	155 Hz: The sensor outputs data 155 times per second.
	•	300 Hz: The sensor outputs data 300 times per second.
	•	560 Hz: The sensor outputs data 560 times per second.
	•	1000 Hz: The sensor outputs data 1000 times per second.

### Gauss range
the maximum magnetic field strength that the sensor can measure:

	•	±4 gauss: The sensor can measure magnetic field strengths from -4 gauss to +4 gauss.
	•	±8 gauss: The sensor can measure magnetic field strengths from -8 gauss to +8 gauss.
	•	±12 gauss: The sensor can measure magnetic field strengths from -12 gauss to +12 gauss.
	•	±16 gauss: The sensor can measure magnetic field strengths from -16 gauss to +16 gauss.

where Gauss (G) is a unit of magnetic flux density or magnetic induction in the centimeter-gram-second (CGS) system of units. (1 gauss is defined as one maxwell per square centimeter). Earth’s magnetic field is approximately 0.25 to 0.65 gauss (25 to 65 microtesla).


## LSM6DS3TRC (Accelerometer & Gyroscope)

### Full-Scale Acceleration Range

The full-scale acceleration range indicates the maximum and minimum values that the accelerometer can measure:

	•	±2 g: The accelerometer can measure acceleration from -2 g to +2 g (±19.62 m/s²)
	•	±4 g: The accelerometer can measure acceleration from -4 g to +4 g (±39.24 m/s²)
	•	±8 g: The accelerometer can measure acceleration from -8 g to +8 g (±58.86 m/s²)
	•	±16 g: The accelerometer can measure acceleration from -16 g to +16 g (±78.48 m/s²)

g (gravity) is the unit of acceleration due to gravity, approximately equal to 9.81 m/s². Therefore, in the ±2 g range, the accelerometer can measure accelerations up to about ±19.62 m/s².


### Angular Rate Range

The angular rate range indicates the maximum and minimum values that the gyroscope can measure:

	•	±125 dps: The gyroscope can measure angular velocities from -125 degrees per second to +125 degrees per second (dps)
	•	±250 dps: The gyroscope can measure angular velocities from -250 degrees per second to +250 degrees per second (dps)
	•	±500 dps: The gyroscope can measure angular velocities from -500 degrees per second to +500 degrees per second (dps)
	•	±1000 dps: The gyroscope can measure angular velocities from -1000 degrees per second to +1000 degrees per second (dps)
	•	±2000 dps: The gyroscope can measure angular velocities from -2000 degrees per second to +2000 degrees per second (dps)


### Data Rates

The data rate determines how frequently the sensor takes measurements and provides data. Here’s an explanation of the different data rates:

#### Accelerometer Data Rates

	•	LSM6DS_RATE_SHUTDOWN (0 Hz): The accelerometer is turned off and does not take any measurements.
	•	LSM6DS_RATE_12_5_HZ (12.5 Hz): The accelerometer takes 12.5 measurements per second.
	•	LSM6DS_RATE_26_HZ (26 Hz): The accelerometer takes 26 measurements per second.
	•	LSM6DS_RATE_52_HZ (52 Hz): The accelerometer takes 52 measurements per second.
	•	LSM6DS_RATE_104_HZ (104 Hz): The accelerometer takes 104 measurements per second.
	•	LSM6DS_RATE_208_HZ (208 Hz): The accelerometer takes 208 measurements per second.
	•	LSM6DS_RATE_416_HZ (416 Hz): The accelerometer takes 416 measurements per second.
	•	LSM6DS_RATE_833_HZ (833 Hz): The accelerometer takes 833 measurements per second.
	•	LSM6DS_RATE_1_66K_HZ (1.66 KHz): The accelerometer takes 1660 measurements per second.
	•	LSM6DS_RATE_3_33K_HZ (3.33 KHz): The accelerometer takes 3330 measurements per second.
	•	LSM6DS_RATE_6_66K_HZ (6.66 KHz): The accelerometer takes 6660 measurements per second.

#### Gyroscope Data Rates

	•	LSM6DS_RATE_SHUTDOWN (0 Hz): The gyroscope is turned off and does not take any measurements.
	•	LSM6DS_RATE_12_5_HZ (12.5 Hz): The gyroscope takes 12.5 measurements per second.
	•	LSM6DS_RATE_26_HZ (26 Hz): The gyroscope takes 26 measurements per second.
	•	LSM6DS_RATE_52_HZ (52 Hz): The gyroscope takes 52 measurements per second.
	•	LSM6DS_RATE_104_HZ (104 Hz): The gyroscope takes 104 measurements per second.
	•	LSM6DS_RATE_208_HZ (208 Hz): The gyroscope takes 208 measurements per second.
	•	LSM6DS_RATE_416_HZ (416 Hz): The gyroscope takes 416 measurements per second.
	•	LSM6DS_RATE_833_HZ (833 Hz): The gyroscope takes 833 measurements per second.
	•	LSM6DS_RATE_1_66K_HZ (1.66 KHz): The gyroscope takes 1660 measurements per second.
	•	LSM6DS_RATE_3_33K_HZ (3.33 KHz): The gyroscope takes 3330 measurements per second.
	•	LSM6DS_RATE_6_66K_HZ (6.66 KHz): The gyroscope takes 6660 measurements per second.




# Comparison between FXOS8700CQ and LSM6DS3TR-C + LIS3MDL

| Sensor                | Type                                      | Accelerometer Range    | Accelerometer Power Consumption | Gyroscope Range                           | Gyroscope Power Consumption | Magnetometer Range               | Magnetometer Power Consumption | Total Power Consumption |
|-----------------------|-------------------------------------------|------------------------|---------------------------------|-------------------------------------------|-----------------------------|-----------------------------------|---------------------------------|-------------------------|
| FXOS8700CQ            | 6-axis (Accelerometer + Magnetometer)     | ±2g, ±4g, ±8g          | 150 µA                          | N/A                                       | N/A                         | ±1200 µT                          | 180 µA                          | 330 µA                  |
| LSM6DS3TR-C + LIS3MDL | 9-axis (Accelerometer + Gyroscope + Magnetometer) | ±2g, ±4g, ±8g, ±16g    | 600 µA                          | ±125, ±245, ±500, ±1000, ±2000 dps        | 1.4 mA                      | ±4, ±8, ±12, ±16 gauss            | 650 µA                          | 2.65 mA                 |
