//
// Created by ipogiba on 10.11.2024.
//

#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <esp_err.h>
#include <soc/gpio_num.h>
#include <ultrasonic.h>

const auto ULTRASONIC_TAG = "ULTRASONIC sensor";

class Ultrasonic {
public:
     Ultrasonic(gpio_num_t hc_sr04_echo, gpio_num_t hc_sr04_trigger);
     esp_err_t read_data(float &distance, const float &tempreature) const;

private:
     ultrasonic_sensor_t ultrasonic_sensor{};
};



#endif //ULTRASONIC_H
