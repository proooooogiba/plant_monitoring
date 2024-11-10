//
// Created by ipogiba on 10.11.2024.
//

#ifndef TEMT6000_H
#define TEMT6000_H
#include <esp_err.h>
#include "driver/adc.h"


class Temt6000 {
public:
  explicit Temt6000(adc1_channel_t channel);
  esp_err_t read_data(float &lux) const;
private:
  adc1_channel_t channel_;
};



#endif //TEMT6000_H
