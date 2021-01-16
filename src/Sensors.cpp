#include "Sensors.h"

Adafruit_AHTX0 temperatureSensor;

bool initiateSensors() {
    Serial.println("\n== [Connecting to sensors] ==\n");

    bool is_aht20_connected = false;

    while (!is_aht20_connected)
    {
        is_aht20_connected = temperatureSensor.begin();
        Serial.println(is_aht20_connected ? "AHT20 connection successful." : "AHT20 connection failed.");

        if(!is_aht20_connected) 
        {
            Serial.println("Trying again in 5 seconds.");
            delay(5000);
        }
    }

    return true;
}