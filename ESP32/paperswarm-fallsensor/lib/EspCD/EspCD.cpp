#include <Arduino.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #define CHIPID ESP.getChipId()
#elif defined(ESP32)
  #include <WiFi.h>
  #include <ESPmDNS.h>
  #define CHIPID (uint32_t) ESP.getEfuseMac()
#else
  #error Platform not supported
#endif
#include <WiFiUdp.h>

#include "EspCD.h"
#include "mydebug.h"

#include <ESPAsyncWebServer.h>


/***************************************
 * Private WS handler for convenience 
 ***************************************/

void _webSocketEvent(ws_callback_t callback,
                     AsyncWebSocket * server, 
                     AsyncWebSocketClient * client, 
                     AwsEventType type, void * arg, 
                     uint8_t *data, size_t len){
  switch (type) {
    case WS_EVT_CONNECT:
      //client connected
      DEBUGF("ws[%s][%u] connect\n", server->url(), client->id());
      client->printf("Hello Client %u :)", client->id());
      client->ping();
      break;
    case WS_EVT_DISCONNECT:
      //client disconnected
      DEBUGF("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
      break;
    case WS_EVT_ERROR:
      //error was received from the other end
      DEBUGF("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
      break;
    case WS_EVT_PONG:
      //pong message was received (in response to a ping request maybe)
      DEBUGF("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
      break;
    case WS_EVT_DATA:
      //data packet
      AwsFrameInfo * info = (AwsFrameInfo*)arg;
      if(info->final && info->index == 0 && info->len == len){
        //the whole message is in a single frame and we got all of it's data
        DEBUGF("ws[%s][%u] %s-message[%llu]: ", server->url(), client->id(), (info->opcode == WS_TEXT)?"text":"binary", len);
        if(info->opcode == WS_TEXT){
          data[len] = 0;
          DEBUGF("%s\n", (char*)data);
          client->text("I got your text message");
        } else {
          callback(len, (char*) data);
          client->binary("I got your binary message");
        }
      } else {

        //message is comprised of multiple frames or the frame is split into multiple packets
        if(info->index == 0){
          if(info->num == 0)
            DEBUGF("ws[%s][%u] %s-message start\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          DEBUGF("ws[%s][%u] frame[%u] start[%llu]\n", server->url(), client->id(), info->num, info->len);
        }

      DEBUGF("ws[%s][%u] frame[%u] %s[%llu - %llu]: ", server->url(), client->id(), info->num, (info->message_opcode == WS_TEXT)?"text":"binary", info->index, info->index + len);
      if(info->message_opcode == WS_TEXT){
        data[len] = 0;
        DEBUGF("%s\n", (char*)data);
      } else {
        for(size_t i=0; i < len; i++){
          DEBUGF("%02x ", data[i]);
        }
        DEBUGF("\n");
      }

      if((info->index + len) == info->len){
        DEBUGF("ws[%s][%u] frame[%u] end[%llu]\n", server->url(), client->id(), info->num, info->len);
        if(info->final){
          DEBUGF("ws[%s][%u] %s-message end\n", server->url(), client->id(), (info->message_opcode == WS_TEXT)?"text":"binary");
          if(info->message_opcode == WS_TEXT)
            client->text("I got your text message");
          else
            client->binary("I got your binary message");
        }
      }
    }
  }
}

/***************************************
 * EspCD classes
 ***************************************/

void EspCD::setup() {
    DEBUG_START;
    DEBUG("Started serial");

    char* ssid = "ESP_xxxxxxxx";
    sprintf(ssid, "ESP_%08X", CHIPID);

    LED_ON;
    DEBUG("Starting wifi AP mode...");
    WiFi.softAP(ssid);
    IPAddress IP = WiFi.softAPIP();
    DEBUG("AP IP address: ", IP);
    LED_OFF;

    MDNS.begin(ssid);
    DEBUG("Setting mDNS hostname: ", ssid);

    ws_callback_t callback = _callback;

    _ws.onEvent([callback](
                     AsyncWebSocket * server, 
                     AsyncWebSocketClient * client, 
                     AwsEventType type, void * arg, 
                     uint8_t *data, size_t len){
        _webSocketEvent(callback, server, client, type, arg, data, len);
    });

    _http.addHandler(&_ws);
    DEBUG("EspCD setup complete!");
}

void EspCD::serve(char *page, http_callback_t callback) {
    _http.on(page, HTTP_GET, callback);
}

void EspCD::begin() {
    _http.begin();
}

void EspCD::loop() {
}
