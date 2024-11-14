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
#include <yl69.h>

#include "dht11.h"
#include "driver/adc.h"

class SensorContainer {
public:
    SensorContainer(); // Constructor

    std::unique_ptr<MQTTClient> mqtt_client;
    std::unique_ptr<DHT11> dht11;
    std::unique_ptr<Ultrasonic> hc_sr04;
    std::unique_ptr<Temt6000> temt6000;
    std::unique_ptr<YL69> yl69;
private:
    static void init_wifi_with_nvs();
};

_Noreturn void run();
std::string marshal_metrics_to_json(float temperature,
    float humidity, float lux, float distance, float soil_moisture);

#endif // SENSOR_CONTAINER_H