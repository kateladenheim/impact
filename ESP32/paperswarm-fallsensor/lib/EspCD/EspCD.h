#pragma once

#include <ESPAsyncWebServer.h>


#define STATIC_GZ(type, pg)                                             \
  [](AsyncWebServerRequest *request){                                   \
    AsyncWebServerResponse *response =                                  \
      request->beginResponse_P(200, "text/" type, pg##_gz, pg##_gz_len);\
    response->addHeader("Content-Encoding", "gzip");                    \
    request->send(response);                                            \
  } 

#define STATIC(type, pg)                                                \
  [](AsyncWebServerRequest *request){                                   \
    AsyncWebServerResponse *response =                                  \
      request->beginResponse_P(200, "text/" type, pg##_txt, pg##_len);  \
    request->send(response);                                            \
  } 

typedef void (*ws_callback_t)(size_t, char *);
typedef void (*http_callback_t)(AsyncWebServerRequest *);

inline void null_callback(size_t rxc, char* message) {
    // do nothing
    return;
};

class EspCD {
    private:
        AsyncWebServer _http;
        AsyncWebSocket _ws;
        ws_callback_t _callback = null_callback;

    public:
        inline EspCD()
            : _http(80), _ws("/ws") {};
        void setup();
        inline void onMessage(ws_callback_t callback) { 
            _callback = callback;
        };
        void serve(char *page, http_callback_t callback);
        void begin();
        void loop();
};
