#include <Adafruit_AHTX0.h>

struct ahtReading {
    float temperature;
    float humidty;
    bool success;
};

bool initiateSensors();
ahtReading getAhtReading();