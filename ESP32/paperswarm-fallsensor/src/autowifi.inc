#include <AutoConnect.h>
#include <mydebug.h>


AutoConnect ac_portal;
AutoConnectConfig ac_config;
char ssid[] = "ESP_xxxxxxxx";

void wifi_setup() {
    sprintf(ssid, "ESP_%08X", CHIPID);
    DEBUG("Starting autoconnect portal on ssid: ", ssid);

    ac_config.apid = ssid;
    ac_config.beginTimeout = 20000; // 20 seconds to connect to wifi before starting AP
    ac_portal.config(ac_config);
    ac_portal.begin();
    DEBUG("Webserver started:", WiFi.localIP().toString());
}

void wifi_loop() {
    ac_portal.handleClient();
}
