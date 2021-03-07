#include "Sensors.h"

Adafruit_AHTX0 aht;

bool initiateSensors() {
    Serial.println("\n== [Connecting to sensors] ==\n");

    bool is_aht20_connected = false;

    while (!is_aht20_connected)
    {
        is_aht20_connected = aht.begin();
        Serial.println(is_aht20_connected ? "AHT20 connection successful." : "AHT20 connection failed.");

        if(!is_aht20_connected) 
        {
            Serial.println("Trying again in 5 seconds.");
            delay(5000);
        }
    }

    return true;
}

ahtReading getAhtReading() {
    // TODO: Add a check to see if the sensor is still connected
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // populate temp and humidity objects with fresh data
    ahtReading reading = {
        .temperature = temp.temperature,
        .humidty = humidity.relative_humidity
    };


    return reading;
}