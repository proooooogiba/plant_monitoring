//
// Created by ipogiba on 04.11.2024.
//

#ifndef SENSOR_CONTAINER_H
#define SENSOR_CONTAINER_H

// #ifdef __cplusplus
// extern "C" {
// #endif

#include <string>

#include "driver/adc.h"
#include "ultrasonic.h"

class SensorContainer {
public:
    SensorContainer(); // Constructor
    // Method to read temperature, humidity, and distance
    void readGpioSensors(float &temperature, float &humidity, float &distance);

    // Method to read lux from the TEMT6000
    float readLux();

private:
    ultrasonic_sensor_t ultrasonicSensor{}; // Instance of the ultrasonic sensor

    // Private methods for configuration
    static void configGpio();
    static void configAdc();
    void config_adc();

    // Method to read lux from ADC
    static float readLuxFromAcd(adc1_channel_t channel);
};

void initWifiWithNvs();
_Noreturn void run();
std::string marshalMetricsToJSON(float temperature, float humidity, float lux, float distance);

#endif // SENSOR_CONTAINER_H