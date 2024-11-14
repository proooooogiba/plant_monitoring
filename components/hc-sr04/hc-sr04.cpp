//
// Created by ipogiba on 10.11.2024.
//

#include "hc-sr04.h"

#include <esp_log.h>
#include <ultrasonic.h>
#define MAX_DISTANCE_CM 400

 Ultrasonic::Ultrasonic(gpio_num_t hc_sr04_trigger, gpio_num_t hc_sr04_echo) {
     ultrasonic_sensor = {
         .trigger_pin = hc_sr04_trigger,
         .echo_pin = hc_sr04_echo,
     };

     if (const auto ultrasonic_init_err = ultrasonic_init(&ultrasonic_sensor);
         ultrasonic_init_err != ESP_OK) {
         ESP_LOGE(ULTRASONIC_TAG, "ultrasonic_init failed %s", esp_err_to_name(ultrasonic_init_err));
     }
}

esp_err_t Ultrasonic::read_data(float &distance, const float &tempreature) const {
     if (const auto read_err = ultrasonic_measure_temp_compensated(&ultrasonic_sensor, MAX_DISTANCE_CM, &distance, tempreature); read_err != ESP_OK) {
         return read_err;
     }
     // так как это расстояние датчик -> объект -> датчик, то делим на 2
     distance /= 2;
     return ESP_OK;
 }
