/*
simple socket over WiFi
version 0.1
date:   10/17/23
*/
#include <WiFi.h>

const char* ssid = "TP-LINK_D128";
const char* password = "43413065";
const char* host = "192.168.0.100"; // Replace with your server's IP address
const uint16_t port = 8080; // Replace with your server's port

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection to server failed");
    delay(1000);
    return;
  }

  Serial.println("Connected to server");
  client.print("Hello from ESP32");

  while (client.connected()) {
    if (client.available()) {
      String line = client.readStringUntil('\n');
      Serial.println(line);
    }
  }

  client.stop();
  delay(1000);
}

