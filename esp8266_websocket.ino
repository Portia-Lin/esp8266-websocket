#include "index.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>

byte output = 2;
byte button = 0;
String web = webpage;
unsigned long timing;
boolean flag = 0;
boolean wsStatus = 0;
char ssid[] = "AstriaPorta";
char pass[]= "69632300369969";

WiFiServer server(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void setup() {
  // put your setup code here, to run once:
  pinMode(output, OUTPUT);
  pinMode(button, INPUT);
  digitalWrite(output, LOW);
  Serial.begin(115200);
  Serial.println();
  Serial.println("***");
  Serial.print("[WIFI] Connecting to ");
  Serial.println("ssid");
  WiFi.begin(ssid, pass);
  int count = 0; 
  while ( (WiFi.status() != WL_CONNECTED) && count < 17) 
  {
      Serial.print(". ");  delay(500);  count++;
  }
 
  if (WiFi.status() != WL_CONNECTED)
  { 
     Serial.println("");
     Serial.print("[WIFI] Failed to connect to ");
     Serial.println(ssid);
     while(1);
  }
  Serial.println("");
  Serial.println("[WIFI] Connected to WiFi");
  Serial.print("[WIFI] IP: ");
  Serial.println(WiFi.localIP()); 

  server.begin();
  webSocket.begin();
  Serial.println("[WS] Server started");
  webSocket.onEvent(webSocketEvent);

}

void loop() {
  // put your main code here, to run repeatedly:
  broadcastState();
  checkButton();
  webSocket.loop();
  WiFiClient client = server.available();     // Check if a client has connected
    if (!client)  {  return;  }
      if (client) {
  }
 
    client.flush();
    client.print( web ); 
 
    delay(5);
}

void webSocketEvent(byte num, WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      wsStatus = 0;
      Serial.print("[WS] Type ");
      Serial.print(type);
      Serial.println(": DISCONNECTED");
      break;
    case WStype_CONNECTED:
      wsStatus = 1;
      Serial.print("[WS] Type ");
      Serial.print(type);
      Serial.println(": CONNECTED");
      if (digitalRead(output) == LOW) {
        webSocket.broadcastTXT("0");
      }
      if (digitalRead(output) == HIGH) {
        webSocket.broadcastTXT("1");
      }
      break;
    case WStype_TEXT:
    Serial.print("[WS] Type ");
      Serial.print(type);
      Serial.println(": TEXT");
      if (payload[0] == 't')
      {
          digitalWrite(output, !digitalRead(output));
          Serial.print("[WS] Payload: ");
          Serial.println(payload[0]);
      }
  }
}

void broadcastState() {
  if (wsStatus == 1 && millis() - timing > 200) {
    timing = millis();
    if (digitalRead(output) == LOW) {
      webSocket.broadcastTXT("0");
    }
    if (digitalRead(output) == HIGH) {
      webSocket.broadcastTXT("1");
    }
  }
}

void checkButton() {
  if (digitalRead(button) == LOW && flag == 0) {
    flag = 1;
    digitalWrite(output, !digitalRead(output));
    }
  if (digitalRead(button) == HIGH && flag == 1) {
    flag = 0;
  }
}
