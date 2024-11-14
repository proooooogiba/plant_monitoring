//
// Created by ipogiba on 10.11.2024.
//

#include "yl69.h"

#include <esp_adc_cal.h>

YL69::YL69(
    const adc1_channel_t _channel,
    const uint32_t _num_of_samples,
    adc_atten_t _atten,
    adc_unit_t _unit
    ) {
    channel = _channel;
    num_of_samples = _num_of_samples;
    esp_adc_cal_characterize(_unit, _atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, &adcCharacteristics);
}

uint32_t YL69::read_raw_data() const {
    return adc1_get_raw(channel);
}

float YL69::normalize(uint32_t value_t) const {
    return (value_t * 100) / value_max;
}


esp_err_t YL69::read_data(float& soil_moisture) const {
    const uint32_t raw_value = read_raw_data();
    soil_moisture = normalize(raw_value);

    return ESP_OK; // Return success status
}




