//
// Created by ipogiba on 10.11.2024.
//

#ifndef YL69_H
#define YL69_H
#include <esp_adc_cal_types_legacy.h>
#include <esp_err.h>
#include <memory>

#include "driver/adc.h"

#define DEFAULT_VREF 1100 // Use adc2_vref_to_gpio() to obtain a better estimate


class YL69 {
public:
	YL69(adc1_channel_t _channel, uint32_t _num_of_samples = 10, adc_atten_t _atten = ADC_ATTEN_DB_12, adc_unit_t _unit = ADC_UNIT_1);
	esp_err_t read_data(float &soil_moisture) const;
private:
	adc1_channel_t channel;
	uint32_t num_of_samples;
	float normalize(uint32_t value_t) const;
	uint32_t read_raw_data() const;
	esp_adc_cal_characteristics_t   adcCharacteristics{};

	float value_max = 4095; // max ADV value of soil mosture
};



#endif //YL69_H
