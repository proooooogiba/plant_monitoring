//
// Created by ipogiba on 10.11.2024.
//

#include "temt6000.h"

#include <hal/adc_types.h>
#include "esp_adc/adc_oneshot.h"

#define TEMT6000_PIN ADC1_CHANNEL_0
#define TEMT6000_POWER_VOLTAGE 3.3

Temt6000::Temt6000(adc1_channel_t channel) {
    channel_ = channel;
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(channel, ADC_ATTEN_DB_0);
 }

esp_err_t Temt6000::read_data(float &lux) const {
    const int adc_value = adc1_get_raw(channel_);
    const float voltage = adc_value * (TEMT6000_POWER_VOLTAGE / 1024); // Convert ADC value to voltage
    lux = (voltage / 10000.0) * 2000000.0; // Convert voltage to lux
    return ESP_OK;
}
