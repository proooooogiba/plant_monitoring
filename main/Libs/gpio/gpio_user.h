//
// Created by ipogiba on 04.11.2024.
//

#ifndef GPIO_USER_H
#define GPIO_USER_H
#include <driver/adc_types_legacy.h>

void run(void);
void config_gpio(void);
void config_adc(void);
float acd_read_lux(adc1_channel_t channel);

#endif