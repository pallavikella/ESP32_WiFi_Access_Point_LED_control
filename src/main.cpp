#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Dwaraka";   // Wi-Fi name ESP32 will create
const char* password = "RadhaKrishna";         // Minimum 8 characters

const int ledPin = 2; // GPIO 2 has onboard LED on many ESP32 boards
bool ledState = false;

WebServer server(80);  // HTTP server on port 80

void handleRoot() {
  String html = "<!DOCTYPE html><html><head><title>ESP32 LED Control</title></head><body>";
  html += "<h1>ESP32 Access Point - LED Control</h1>";
  html += "<p>LED is currently: <strong>" + String(ledState ? "ON" : "OFF") + "</strong></p>";
  html += "<p><a href=\"/on\"><button>Turn ON</button></a></p>";
  html += "<p><a href=\"/off\"><button>Turn OFF</button></a></p>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleLedOn() {
  digitalWrite(ledPin, HIGH);
  ledState = true;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleLedOff() {
  digitalWrite(ledPin, LOW);
  ledState = false;
  server.sendHeader("Location", "/");
  server.send(303);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  WiFi.softAP(ssid, password); // ESP32 starts AP
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);  // Usually 192.168.4.1

  server.on("/", handleRoot);
  server.on("/on", handleLedOn);
  server.on("/off", handleLedOff);

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
