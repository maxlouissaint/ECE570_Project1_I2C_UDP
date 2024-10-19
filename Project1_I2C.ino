/*
Project 1: I2C interface to MPU6050 Gyroscope sensor
Version: 0.1
Date: 8/27/2023 
*/
#include<Wire.h>
const uint8_t addr = 0x68;
size_t L = 14;
int16_t xA, yA, zA, T, xG, yG, zG;
float xAcc, yAcc, zAcc, Temp, xGyr, yGyr, zGyr;

void setup(){
  Wire.begin();
  Wire.beginTransmission(addr);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);
}

void loop(){
  Wire.beginTransmission(addr);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(addr, L, true);
  
  xA = (Wire.read()<<8|Wire.read());
  yA = (Wire.read()<<8|Wire.read()); 
  zA = (Wire.read()<<8|Wire.read());
  T =  (Wire.read()<<8|Wire.read());
  xG = (Wire.read()<<8|Wire.read());
  yG = (Wire.read()<<8|Wire.read());
  zG = (Wire.read()<<8|Wire.read());

  xAcc = xA/16384.0;
  yAcc = yA/16384.0;
  zAcc = zA/16384.0;
  xGyr = xG/131.0;
  yGyr = yG/131.0;
  zGyr = zG/131.0;
  Temp = T/340.0 + 36.53;

  //Roll & Pitch Equations from accelerometers only
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
  delay(100);
}
