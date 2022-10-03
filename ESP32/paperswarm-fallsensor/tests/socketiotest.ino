#define DEBUG_ESP_PORT Serial

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <SocketIOclient.h>

SocketIOclient socketIO;

void socketIOEvent(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    Serial.print("Msg: ");      Serial.println(type);
    Serial.print("Payload: ");  Serial.println((char*) payload);
    Serial.print("Length: ");   Serial.println(length);
}

void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.setDebugOutput(true);

    Serial.println("\n\n\n=== Start logging ===\n");

    WiFi.begin("torii", "6283185307");
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print("~");
        delay(100);
    };

    Serial.println("Webserver started:" + WiFi.localIP().toString());

    socketIO.beginSSL("esphub.mehtank.com", 443, "/socket.io/?EIO=4");
    socketIO.onEvent(socketIOEvent);
}

void loop() {
    socketIO.loop();

    Serial.print(".");
    delay(50);
}
