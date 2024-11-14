#include "temt6000.h"

#include <esp_adc_cal.h>
#include <driver/adc.h>
#include <esp_err.h>

#define TEMT6000_PIN ADC1_CHANNEL_0
#define TEMT6000_POWER_VOLTAGE 3.3
#define SAMPLES 10  // Количество сэмплов для усреднения


/** Output range of configured ADC unit */
#define TEMT6000_ADC_WIDTH                          ADC_WIDTH_BIT_12
/** Default reference voltage of configured ADC unit */
#define TEMT6000_ADC_ATTEN_VREF                     3900

/** Coefficient used for illuminance convertion from read output in voltage */
#define TEMT6000_ILLUMINANCE_CONVERTER_VALUE        0.2f
/** Specifies the value of lower margin reduction */
#define TEMT6000_ADC_MARGIN_LOW_REDUCED_VALUE       0


// Конструктор с инициализацией ADC
Temt6000::Temt6000(adc1_channel_t _channel) {
    channel = _channel;
    esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_0, ADC_WIDTH_BIT_10, TEMT6000_ADC_ATTEN_VREF, &adcCharacteristics);
}

// Метод для считывания данных
esp_err_t Temt6000::read_data(float &lux) const {
    const int adc_value = adc1_get_raw(channel);
    const float voltage = adc_value * (TEMT6000_POWER_VOLTAGE / 1024); // Convert ADC value to voltage
    lux = 2640 - (voltage / 10000.0) * 2000000.0; // Convert voltage to lux
    return ESP_OK;
}