//
// Created by ipogiba on 10.11.2024.
//

#include "dht11.h"

#include <hal/gpio_types.h>
#include <soc/gpio_num.h>
#include "dht.h"
#include "driver/gpio.h"

DHT11::DHT11(const gpio_num_t gpio_num) {
    data_dht_num = gpio_num;
    gpio_set_pull_mode(data_dht_num, GPIO_PULLUP_ONLY);
}

esp_err_t DHT11::read_data(float &temperature, float &humidity) const {
    return dht_read_float_data(SENSOR_TYPE, data_dht_num, &humidity, &temperature);
}
