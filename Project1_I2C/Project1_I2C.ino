/*
Project 1: I2C interface to MPU-6050 6 DOF 3 Axis Accelerometer Gyroscope Sensor Module GY-521 by HiLetgo
Sensor Spec: https://hiletgo.com/ProductDetail/2157948.html
Version: 1.0
Date: 8/27/2023 
*/
#include<Wire.h> //library for facilitating I2C communication between 2 devices

const uint8_t addr = 0x68; //sensor (slave device) address
size_t L = 14; //number of bytes to read from sensor
int16_t xA, yA, zA, T, xG, yG, zG; //store raw data from sensor
float xAcc, yAcc, zAcc, Temp, xGyr, yGyr, zGyr; //converted sensor data

void setup(){
  Wire.begin(); //initialize I2C communication
  Wire.beginTransmission(addr); //start communicating with sensor
  Wire.write(0x6B); // write to power management register to put sensor to active mode
  Wire.write(0); //value 0 is written to the register to disable sleep mode
  Wire.endTransmission(true); //end transmission. true will send a stop message, releasing the bus after transmission
  Serial.begin(115200); //initialize serial communication
}

void loop(){
  Wire.beginTransmission(addr); //start communicating with sensor
  Wire.write(0x3B); // read data from register 0x3B (Accelerometer and Gyroscope data start at 0x3B)
  Wire.endTransmission(false); //false will send a restart, keeping the connection active.
  Wire.requestFrom(addr, L, true); //requests L bytes of data from the sensor
  
  /*
  The Wire.read() function reads two bytes at a time (high byte and low byte) for each 
  axis of the accelerometer (xA, yA, zA), the temperature (T), and the gyroscope (xG, yG, zG).
  The two bytes are combined using bit-shifting and bitwise OR to form a single int16_t value for each axis
  */
  xA = (Wire.read()<<8|Wire.read());
  yA = (Wire.read()<<8|Wire.read()); 
  zA = (Wire.read()<<8|Wire.read());
  T =  (Wire.read()<<8|Wire.read());
  xG = (Wire.read()<<8|Wire.read());
  yG = (Wire.read()<<8|Wire.read());
  zG = (Wire.read()<<8|Wire.read());

  xAcc = xA/16384.0; // Convert accelerometer readings to g (16384 for ±2g range)
  yAcc = yA/16384.0; 
  zAcc = zA/16384.0; 
  xGyr = xG/131.0; // Convert gyroscope readings to degrees per seconds (131 for ±250 degrees/sec)
  yGyr = yG/131.0;
  zGyr = zG/131.0;
  Temp = T/340.0 + 36.53; // Convert temperature to Celsius

  //Roll & Pitch Equations from accelerometers only to determine the orientation of the device in three-dimensional space.
  float roll  = atan2(yAcc, zAcc) * 180.0/M_PI;
  float pitch = atan2(-xAcc, sqrt(zAcc * zAcc + yAcc * yAcc)) * 180.0/M_PI;
 
  Serial.print(xAcc); Serial.print(",");
  Serial.print(yAcc); Serial.print(",");
  Serial.print(zAcc); Serial.print(",");
  Serial.print(xGyr); Serial.print(",");
  Serial.print(yGyr); Serial.print(",");
  Serial.print(zGyr); Serial.print(",  ");
  Serial.print(Temp); Serial.print(", ");
  Serial.print(roll); Serial.print(",  ");
  Serial.print(pitch); Serial.print(",  ");
  Serial.println();
  //After combining the high byte and low byte, xA = xA/16384.0, xG = xG/131.0, and T = T/340.00 + 36.53
  delay(100); // Wait for 100 milliseconds before the next reading
}
