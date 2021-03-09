#include "Sensors.h"

Adafruit_AHTX0 aht;

bool initiateSensors() {
    Serial.println("\n== [Connecting to sensors] ==\n");

    bool is_aht20_connected = false;

    while (!is_aht20_connected) {
        is_aht20_connected = aht.begin();
        Serial.println(is_aht20_connected ? "AHT20 connection successful.\n" : "AHT20 connection failed.\n");

        if(!is_aht20_connected) {
            Serial.println("Trying again in 5 seconds.");
            delay(5000);
        }
    }

    return true;
}

ahtReading getAhtReading() {
    // TODO: Add a check to see if the sensor is still connected
    ahtReading reading = {
        .temperature = 0.0F,
        .humidty = 0.0F,
        .success = false
    };

    if (aht.getStatus() == 0xFF) {
        Serial.println("AHT20 isn't connected, check your wiring!");
        return reading;
    }
    
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp);
    reading = {
        .temperature = temp.temperature,
        .humidty = humidity.relative_humidity,
        .success = true
    };

    return reading;
}