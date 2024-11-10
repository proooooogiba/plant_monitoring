//
// Created by ipogiba on 04.11.2024.
//

#ifndef SENSOR_CONTAINER_H
#define SENSOR_CONTAINER_H

// #ifdef __cplusplus
// extern "C" {
// #endif

#include <memory>
#include <mqtt.hh>
#include <string>

#include "driver/adc.h"
#include "ultrasonic.h"

class SensorContainer {
public:
    SensorContainer(); // Constructor
    // Method to read temperature, humidity, and distance
    void read_gpio_sensors(float &temperature, float &humidity, float &distance) const;

    // Method to read lux from the TEMT6000
    static float read_lux();

    std::unique_ptr<MQTTClient> mqtt_client;
private:
    ultrasonic_sensor_t ultrasonic_sensor{}; // Instance of the ultrasonic sensor

    // Private methods for configuration
    static void config_dht11_gpio(gpio_num_t gpio_num);
    static void config_adc();
    static void init_wifi_with_nvs();

    // Method to read lux from ADC
    static float read_lux_from_acd(adc1_channel_t channel);
};

_Noreturn void run();
std::string marshal_metrics_to_json(float temperature, float humidity, float lux, float distance);

#endif // SENSOR_CONTAINER_H