/*
Project 1: I2C interface to MPU-6050 6 DOF 3 Axis Accelerometer Gyroscope Sensor Module GY-521 by HiLetgo
Sensor Spec: https://hiletgo.com/ProductDetail/2157948.html
Version: 1.0
Date: 8/27/2023 
*/
#include<Wire.h>  // library for facilitating I2C communication between 2 devices
#include <WiFi.h> // WiFi library for ESP32

// The ssid and password of the WIFI network the ESP32 connects to.
const char* ssid = "MOTO7DD2";
const char* password = "u44aa7k5nx";

/*
  Configures the UDP communication by specifying the 
  IP address of the server (your computer)
*/
const char * udpAddress = "192.168.0.21"; //the IP address of the computer in local WiFi 
//found using ifconfig on mac OS under "en0 -> inet ->"

//speficying computer port number
const int udpPort = 6000;
WiFiUDP udp; // Create WIFI UDP instance

uint8_t ack;

const uint8_t addr = 0x68; //sensor (slave device) address
size_t L = 14; //number of bytes to read from sensor
int16_t xA, yA, zA, T, xG, yG, zG; //store raw data from sensor
float xAcc, yAcc, zAcc, Temp, xGyr, yGyr, zGyr; //converted sensor data

void setup(){
  Serial.begin(115200); //initialize serial communication
  Serial.setDebugOutput(true);
  Serial.println();

  //Setup WIFI
  WiFi.begin(ssid, password); //start wifi connection
  while (WiFi.isConnected() != true) { //check connection status. keep checking until connected
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
  
   //Setup I2C
  Wire.begin(); //initialize I2C communication

  //Initialize Sensor connection
  Wire.beginTransmission(addr); //start communicating with sensor
  Wire.write(0x6B); // write to power management register to put sensor to active mode
  Wire.write(0); //value 0 is written to the register to disable sleep mode
  Wire.endTransmission(true); //end transmission. true will send a stop message, releasing the bus after transmission
}

void loop(){
  Wire.beginTransmission(addr); //start communicating with sensor
  Wire.write(0x3B); // read data from register 0x3B (Accelerometer and Gyroscope data start at 0x3B)
  Wire.endTransmission(false); //false will send a restart, keeping the connection active.
  Wire.requestFrom(addr, L, true); //requests L bytes of data from the sensor
  
  /*
  The Wire.read() function reads two bytes at a time (high byte and low byte) for each 
  axis of the accelerometer (xA, yA, zA), the temperature (T), and the gyroscope (xG, yG, zG) from register 0x3B to 0x48.
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
 
  // Serial.print(xAcc); Serial.print(",");
  // Serial.print(yAcc); Serial.print(",");
  // Serial.print(zAcc); Serial.print(",");
  // Serial.print(xGyr); Serial.print(",");
  // Serial.print(yGyr); Serial.print(",");
  // Serial.print(zGyr); Serial.print(",  ");
  // Serial.print(Temp); Serial.print(", ");
  Serial.print("Roll: ");Serial.print(roll); Serial.print(",  ");
  Serial.print("Pitch: ");Serial.print(pitch);
  Serial.println();


  // // Pack roll and pitch into a byte buffer for sending data
  uint8_t buffer[8]; // Buffer to hold packed roll and pitch data 8 bytes
  packFloatToBytes(roll, buffer, 0);     // Pack roll at index 0
  packFloatToBytes(pitch, buffer, 4);    // Pack pitch at index 4

// Print the content of the buffer in binary format for debugging
  Serial.print("Buffer content (binary): ");
  for (int i = 0; i < 8; i++) {
    Serial.print(buffer[i], BIN); // Print each byte in binary
    Serial.print(" ");
  }
  Serial.println();
  // packFloatToBytes(xAcc, buffer, 8);     // Pack xAcc at index 8
  // packFloatToBytes(yAcc, buffer, 12);    // Pack yAcc at index 12
  // packFloatToBytes(zAcc, buffer, 16);    // Pack zAcc at index 16
  // packFloatToBytes(xGyr, buffer, 20);    // Pack xGyr at index 20
  // packFloatToBytes(yGyr, buffer, 24);    // Pack yGyr at index 24
  // packFloatToBytes(zGyr, buffer, 28);    // Pack zGyr at index 28
  // packFloatToBytes(Temp, buffer, 32);    // Pack Temp at index 32

  //UDP Sending data
  udp.beginPacket(udpAddress, udpPort);
  udp.write(buffer, 8); //send only 8 bytes
  udp.endPacket();
  delay(100);

  memset(buffer, 0, 8); //clear buffer for the next cycle

  udp.parsePacket();
  if(udp.read(buffer, 1) > 0){ //checks if there is an incoming packet from the server
    Serial.print("Server to client: ");
    ack = buffer[0]; //If a packet is received, the first byte is stored in ack
    Serial.println((String)ack);
  }
  
  delay(10); /// Wait for 10 milliseconds before the next reading
}

// Function to pack a float into 4 bytes using bitwise operations
// Uses '>': big-endian byte order, where MSB is stored first (lowest memory location)
void packFloatToBytes(float value, uint8_t* buffer, int startIndex) {
  uint32_t temp = *((uint32_t*)&value); // Cast float to uint32_t (32-bit integer) hen dereference to get the bits
  buffer[startIndex] = (temp >> 24) & 0xFF; // Extract most significant byte
  buffer[startIndex + 1] = (temp >> 16) & 0xFF; // Extract second byte
  buffer[startIndex + 2] = (temp >> 8) & 0xFF; // Extract third byte
  buffer[startIndex + 3] = temp & 0xFF; // Extract least significant byte
}

// Function to unpack 4 bytes from the buffer into a float
float unpackBytesToFloat(uint8_t* buffer, int startIndex) {
  uint32_t temp = 0;
  temp |= (buffer[startIndex] << 24);
  temp |= (buffer[startIndex + 1] << 16);
  temp |= (buffer[startIndex + 2] << 8);
  temp |= buffer[startIndex + 3];
  
  float value = *((float*)&temp); // Cast the 32-bit integer back to a float
  return value;
}