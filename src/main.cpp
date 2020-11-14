#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

// Access Point Configuration
const char* ssid = "PlantKit";
const char* password = "plantparty";

IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);

void handle_index() {
	String html = "<!doctype html>\
	<html>\
		<body>\
			<form action=\"/wifi\" method=\"get\">\
				<label for=\"ssid\">SSID: </label>\
				<input type=\"text\" id=\"ssid\" name=\"ssid\"><br><br>\
				<label for=\"password\">Password:</label>\
				<input type=\"text\" id=\"password\" name=\"password\"><br><br>\
				<input type=\"submit\" value=\"Submit\">\
			</form>\
		</body>\
	</html>";
	server.send(200, "text/html", html); 
}

void handle_wifi() {
	if(server.hasArg("ssid") && server.hasArg("password")){
		String ssid = server.arg("ssid");
		String password = server.arg("password");
		server.send(200, "text/plain", "SSID: '" + ssid + "' - Password: '" + password + "'");
	} else {
  		server.send(404, "text/plain", "404 Not found");
	}
}

void handle_reset() {
	String html = "<!doctype html>\
	<html>\
		<body>\
			<p>ToDo: Reset WiFi</p>\
		</body>\
	</html>";
	server.send(200, "text/html", html); 
}

void setup() {
	Serial.begin(9600);
	WiFi.mode(WIFI_AP);

	WiFi.softAP(ssid, password);
	delay(100); // https://github.com/espressif/arduino-esp32/issues/985
	WiFi.softAPConfig(local_ip, gateway, subnet);
	
	server.on("/", handle_index);
	server.on("/wifi", handle_wifi);
  	server.on("/reset", handle_reset);

	server.begin();
	Serial.println("PlantKit web server started!");
}


void loop() {
	// put your main code here, to run repeatedly:
	server.handleClient();
}