//
// Created by ipogiba on 04.11.2024.
//

#ifndef SENSOR_CONTAINER_H
#define SENSOR_CONTAINER_H

#include <hc-sr04.h>
#include <temt6000.h>

#include <memory>
#include <mqtt.hh>
#include <string>

#include "dht11.h"
#include "driver/adc.h"

class SensorContainer {
public:
    SensorContainer(); // Constructor
    // Method to read temperature, humidity, and distance
    void read_gpio_sensors(float &temperature, float &humidity, float &distance) const;

    // Method to read lux from the TEMT6000
    static float read_lux();

    std::unique_ptr<MQTTClient> mqtt_client;
    std::unique_ptr<DHT11> dht11;
    std::unique_ptr<Ultrasonic> hc_sr04;
    std::unique_ptr<Temt6000> temt6000;
private:
    // Private methods for configuration
    static void config_adc();
    static void init_wifi_with_nvs();

    // Method to read lux from ADC
    static float read_lux_from_acd(adc1_channel_t channel);
};

_Noreturn void run();
std::string marshal_metrics_to_json(float temperature, float humidity, float lux, float distance);

#endif // SENSOR_CONTAINER_H