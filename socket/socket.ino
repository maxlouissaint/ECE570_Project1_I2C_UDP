/*
Simple socket over WiFi - 
This code allows an ESP32 microcontroller to connect to a WiFi network 
and establish a TCP connection to a server running on a specified IP 
address and port. It sends a message to the 
server and reads any responses from it.
version 0.1
date:   10/17/23 updated on 10/19/24
*/
#include <WiFi.h>

// The ssid and password of the WIFI network the ESP32 connects to.
const char* ssid = "MOTO7DD2";
const char* password = "u44aa7k5nx";

/*
  Configures the UDP communication by specifying the 
  IP address of the server (your computer)
*/
const char* host = "192.168.0.21"; // server IP address

//use "netstat -an | grep LISTEN" on MAC to find an open port to connect to
const uint16_t port = 8080 ; // port number the server is listening on 

void setup() {
  Serial.begin(115200); //Initializes serial communication at a baud rate of 115200
  WiFi.begin(ssid, password); //start connection to the wifii network

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  WiFiClient client; //instantiate a wifi client object. ESP32 will act as the client

  if (!client.connect(host, port)) { //connect client to server. If return false print connection error
    Serial.println("Connection to server failed");
    delay(1000);
    return;
  }

  Serial.println("Connected to server");
  client.print("Hello from ESP32"); //client send message to server

  // 
  while (client.connected()) {
    if (client.available()) { //check to see if there's data available from the server
      String line = client.readStringUntil('\n'); //read data until a new line
      Serial.println(line); //print to serial monitor
    }
  }

  client.stop(); //end the connection with the server when done
  delay(1000);
}

