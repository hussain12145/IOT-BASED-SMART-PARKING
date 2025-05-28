#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h> 

// Communication with Arduino
#define RX_PIN D2  // GPIO4
#define TX_PIN D3  // GPIO0
SoftwareSerial mySerial(RX_PIN, TX_PIN);

// Wi-Fi credentials
const char* ssid = "##########"; // REPLACE YOUR SSID
const char* password = "#########"; // REPLACE YOUR PSS

// Google Apps Script Web App URL
const char* serverURL = "https://script.google.com/macros/s/**************************/exec"; //REPLACE YOU WEB APP ID HERE 

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected!");
}

void loop() {
  if (mySerial.available()) {
    String input = mySerial.readStringUntil('\n');
    input.trim();
    Serial.println("Received: " + input);

    if (input.startsWith("ENTRY:") || input.startsWith("EXIT:")) {
      processAndUpload(input);
    }
  }
}

void processAndUpload(String data) {
  String type = data.startsWith("ENTRY:") ? "ENTRY" : "EXIT";
  data = data.substring(type.length() + 1);  

  int firstColon = data.indexOf(':');
  int secondColon = data.indexOf(':', firstColon + 1);

  String uid = data.substring(0, firstColon);
  String availableSlots = data.substring(firstColon + 1, secondColon);
  String assignedSlot = data.substring(secondColon + 1);

  String url = String(serverURL) + "?uid=" + uid + "&type=" + type +
               "&availableSlots=" + availableSlots + "&assignedSlot=" + assignedSlot;

  WiFiClientSecure client;
  client.setInsecure();  

  HTTPClient http;
  http.begin(client, url);  
  int httpCode = http.GET();

  if (httpCode == 200) {
    String response = http.getString();
    Serial.println("HTTP Error: " + response);
  } else {
    Serial.println("Success: " + String(httpCode));
  }

  http.end();
}
