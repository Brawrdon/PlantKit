#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h>
#include <AutoConnect.h>
#include <ESP32HomeKit.h>
#include "Sensors.h"

WebServer server; 

AutoConnect portal(server);
AutoConnectConfig config;

hap_serv_t *service;
bool hapStarted = false;

/* Mandatory identify routine for the accessory.
 * In a real accessory, something like LED blink should be implemented
 * got visual identification
 */
static int identify(hap_acc_t *ha)
{
	ESP_LOGI(TAG, "Accessory identified");
	return HAP_SUCCESS;
}

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

	hap_acc_t *accessory;

	/* Configure HomeKit core to make the Accessory name (and thus the WAC SSID) unique,
     * instead of the default configuration wherein only the WAC SSID is made unique.
     */
	hap_cfg_t hap_cfg;
	hap_get_config(&hap_cfg);
	hap_cfg.unique_param = UNIQUE_NAME;
	hap_set_config(&hap_cfg);

	/* Initialise the HAP core */
	hap_init(HAP_TRANSPORT_WIFI);

	/* Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
	hap_acc_cfg_t cfg = {
		.name = "PlantKit",
		.model = "Nursery Pot",
		.manufacturer = "Brawrdon",
		.serial_num = "000000000001",
		.fw_rev = "1.0.0",
		.hw_rev = NULL,
		.pv = "1.1.0",
		.cid = HAP_CID_OTHER,
		.identify_routine = identify,
	};

	/* Create accessory object */
	accessory = hap_acc_create(&cfg);

	/* Add a dummy Product Data */
	uint8_t product_data[] = {'P', 'L', 'A', 'N', 'T', 'K', 'I', 'T'};
	hap_acc_add_product_data(accessory, product_data, sizeof(product_data));

	/* Create the Fan Service. Include the "name" since this is a user visible service  */
	service = hap_serv_temperature_sensor_create(10.0);
	hap_serv_add_char(service, hap_char_name_create("PlantKit"));

	/* Add the Fan Service to the Accessory Object */
	hap_acc_add_serv(accessory, service);

	/* Add the Accessory to the HomeKit Database */
	hap_add_accessory(accessory);

	/* Query the controller count (just for information) */
	ESP_LOGI(TAG, "Accessory is paired with %d controllers", hap_get_paired_controller_count());


	/* Hardware initialisation */
    initiateSensors();

	/* Unique Setup code of the format xxx-xx-xxx. Default: 111-22-333 */
	hap_set_setup_code("111-22-333");
	/* Unique four character Setup Id. Default: ES32 */
	hap_set_setup_id("PLKT");

	/* After all the initialisations are done, start the HAP core */
	if(hap_start() == HAP_SUCCESS)
		hapStarted = true;
	
}

void loop() {
	// TODO: Run portal on a separate thread so delay doesn't impact setup
	portal.handleClient();

	if (hapStarted) {
		delay(5000);
		hap_char_t *temp = hap_serv_get_char_by_uuid(service, HAP_CHAR_UUID_CURRENT_TEMPERATURE);
		ahtReading reading = getAhtReading();
		hap_val_t value = {
			.f = reading.temperature
		};

		hap_char_update_val(temp, &value);
	}

	// TODO: Add proper logging for debugging
}