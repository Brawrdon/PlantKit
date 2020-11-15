#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h>
#include <AutoConnect.h>

WebServer server; 

AutoConnect portal(server);
AutoConnectConfig config;


void setup(){
	delay(1000);
	Serial.begin(115200);
	// Portal config
	config.apid = "PlantKit";
	config.psk = "plantparty";
	config.hostName = "PlantKit";
	portal.config(config);

	if (portal.begin()) {
		Serial.println("WiFi connected: " + WiFi.localIP().toString());

		if (WiFi.getMode() == WIFI_MODE_APSTA) {
			ESP.restart();
		}
	}
}

void loop() {
	portal.handleClient();
}