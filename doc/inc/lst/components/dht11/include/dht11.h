//
// Created by ipogiba on 10.11.2024.
//

#ifndef DHT11_H
#define DHT11_H
#include <esp_err.h>
#include <soc/gpio_num.h>

#define SENSOR_TYPE DHT_TYPE_DHT11
const auto DHT_TAG = "DHT sensor";

class DHT11 {
public:
    explicit DHT11(gpio_num_t gpio_num);
    esp_err_t read_data(float &temperature, float &humidity) const;
private:
    gpio_num_t data_dht_num;
};


#endif //DHT11_H
