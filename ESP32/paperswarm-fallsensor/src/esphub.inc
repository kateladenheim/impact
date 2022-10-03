#include <mydebug.h>
#include <SocketIOclient.h>
#include <ArduinoJson.h>
#include <Preferences.h>


#if defined(ARDUINO_ARCH_ESP32)
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
    const uint8_t* src = (const uint8_t*) mem;
    DEBUGF("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
    for(uint32_t i = 0; i < len; i++) {
        if(i % cols == 0) {
            DEBUGF("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
        }
        DEBUGF("%02X ", *src);
        src++;
    }
    DEBUGF("\n");
}
#endif


SocketIOclient sio;
Preferences *esphub_prefs;

DynamicJsonDocument esphub_registration(1024);

void (*esphub_blink)(int);
void (*esphub_config)(Preferences*);
void (*esphub_setpins)(JsonArray);

void esphub_announce(char* event, char* room) {
    DynamicJsonDocument announcement(1024);
    JsonArray array = announcement.to<JsonArray>();
    array.add("announce");
    JsonObject param = array.createNestedObject();
    param["event"] = event;
    param["room"] = room;

    // JSON to String (serializion)
    String json;
    serializeJson(announcement, json);

    // Send event
    sio.sendEVENT(json);

    // Print JSON for debugging
    DEBUG("Sending announcement: ", json);
}

void esphub_register() {
    JsonObject param1 = esphub_registration[1];

    param1["mac"] = CHIPID;
    param1["ip"] = WiFi.localIP().toString();
    param1["name"] = esphub_prefs->getString("name", "ESP device");
    param1["config"] = esphub_prefs->getString("config", "none");

    // JSON to String (serializion)
    String json;
    serializeJson(esphub_registration, json);

    // Send event
    sio.sendEVENT(json);

    // Print JSON for debugging
    DEBUG("Sending registration: ", json);
}

void esphub_handle(uint8_t * payload) {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, payload);
    const char* event = doc[0];
    if (!strcmp(event, "blink")) {
        DEBUG("Blinking LED");
        if (doc.size() > 1)
            esphub_blink(doc[1]);
        else
            esphub_blink(0);
    } else if (!strcmp(event, "pins")) {
        JsonArray value = doc[1].as<JsonArray>();
        DEBUG("Setting pin values: ", value.size());
        esphub_setpins(value);
    } else if (!strcmp(event, "name") || !strcmp(event, "config")) {
        String value = doc[1].as<String>();
        DEBUG("Setting value of: ", event);
        DEBUG("  -> ", value);
        esphub_prefs->putString(event, value);

        if (!strcmp(event, "config")) esphub_config(esphub_prefs);

        esphub_register();
    }
}

void esphub_event(socketIOmessageType_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case sIOtype_DISCONNECT:
            DEBUGF("[IOc] Disconnected!\n");
            break;
        case sIOtype_CONNECT:
            DEBUGF("[IOc] Connected to url: %s\n", payload);
            // join default namespace (no auto join in Socket.IO V3)
            sio.send(sIOtype_CONNECT, "/");
            esphub_register();
            break;
        case sIOtype_EVENT:
            DEBUGF("[IOc] get event: %s\n", payload);
            esphub_handle(payload);
            break;
        case sIOtype_ACK:
            DEBUGF("[IOc] get ack: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_ERROR:
            DEBUGF("[IOc] get error: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_EVENT:
            DEBUGF("[IOc] get binary: %u\n", length);
            hexdump(payload, length);
            break;
        case sIOtype_BINARY_ACK:
            DEBUGF("[IOc] get binary ack: %u\n", length);
            hexdump(payload, length);
            break;
    }
}

void esphub_setup(const char* url, Preferences *prefs, 
        void (*blink)(int),
        void (*config)(Preferences*),
        void (*setpins)(JsonArray)
    ) {

    esphub_prefs = prefs;
    esphub_blink = blink;
    esphub_config = config;
    esphub_setpins = setpins;

    // create JSON message for Socket.IO (event)
    JsonArray array = esphub_registration.to<JsonArray>();

    // add event name
    // Hint: socket.emit('event_name', ....
    array.add("register");

    // add payload (parameters) for the event
    array.createNestedObject();

    sio.beginSSL(url, 443, "/socket.io/?EIO=4", "");
    sio.onEvent(esphub_event);
}

void esphub_loop() {
    sio.loop();
}
