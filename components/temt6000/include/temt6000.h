//
// Created by ipogiba on 10.11.2024.
//

#ifndef TEMT6000_H
#define TEMT6000_H
#include <esp_adc_cal_types_legacy.h>
#include <esp_err.h>
#include <memory>


const auto LIGHT_TAG = "LIGHT sensor";

class Temt6000 {
public:
  Temt6000(adc1_channel_t _channel);
  esp_err_t read_data(float &lux) const;
private:
  adc1_channel_t channel;
  esp_adc_cal_characteristics_t   adcCharacteristics; /**< Characteristics of selected ADC unit */
  uint32_t samplesNo = 10;
};

#endif //TEMT6000_H
