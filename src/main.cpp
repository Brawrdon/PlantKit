#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h> 
#include <AutoConnect.h>

WebServer server; 

AutoConnect portal(server);
AutoConnectConfig config;


void rootPage(){
	char content[] = "Hello, world";
	server.send(200, "text/plain", content);
}

void setup(){
	delay(1000);
	Serial.begin(115200);
	Serial.println();

	// Server routes
	server.on("/", rootPage);

	// Portal config
	config.apid = "PlantKit";
	config.psk = "plantparty";
	config.hostName = "PlantKit";
	portal.config(config);
	if (portal.begin()) {
		Serial.println("WiFi connected: " + WiFi.localIP().toString());
	}
}

void loop() {
	portal.handleClient();
}