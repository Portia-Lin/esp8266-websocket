#include "index.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WebSocketsServer.h>

byte output = 2;
byte button = 0;
String web = webpage;
boolean flag = 0;

ESP8266WebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  // put your setup code here, to run once:
  pinMode(output, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(output, LOW);
  Serial.begin(115200);
  Serial.println();
  Serial.println("***");
  Serial.println("Waiting for reset wifi!");
  for (int i = 0; i < 4; i++) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  WiFi.hostname("ESP8266");
  WiFiManager wifiManager;
  if(digitalRead(0) == LOW) {
    wifiManager.resetSettings();
    Serial.println("Reset saved settings!");
  }
  wifiManager.setTimeout(180);
  wifiManager.autoConnect("ESP8266");
  Serial.print("Server IP: ");
  Serial.println(WiFi.localIP()); 
  server.on("/", []() {
    server.send(200, "text/html", web);
  });
  server.begin();
  webSocket.begin();
  Serial.println("[WS] Server started");
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  // put your main code here, to run repeatedly:
  checkButton();
  server.handleClient();
  webSocket.loop();
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.print("[WS] Type ");
      Serial.print(type);
      Serial.println(": DISCONNECTED");
      break;
    case WStype_CONNECTED:
      broadcastState();
      Serial.print("[WS] Type ");
      Serial.print(type);
      Serial.println(": CONNECTED");
      break;
    case WStype_TEXT:
    Serial.print("[WS] Type ");
      Serial.print(type);
      Serial.println(": TEXT");
      if (payload[0] == 't') {
        digitalWrite(output, !digitalRead(output));
        broadcastState();
        Serial.print("[WS] Payload: ");
        Serial.println(payload[0]);
      }
  }
}

void broadcastState() {
  if (digitalRead(output) == HIGH) {
    webSocket.broadcastTXT("0");
  }
  if (digitalRead(output) == LOW) {
    webSocket.broadcastTXT("1");
  }
}

void checkButton() {
  if (digitalRead(button) == LOW && flag == 0) {
    flag = 1;
    digitalWrite(output, !digitalRead(output));
    broadcastState();
    }
  if (digitalRead(button) == HIGH && flag == 1) {
    flag = 0;
  }
}
