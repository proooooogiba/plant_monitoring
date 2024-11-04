//
// Created by ipogiba on 04.11.2024.
//

#include "gpio_user.h"

#include "driver/gpio.h"
#include "hal/gpio_types.h"

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <soc/io_mux_reg.h>
#include "esp_log.h"
#include <dht.h>
#include "driver/adc.h"
#include <ultrasonic.h>

# define BLINK_GPIO GPIO_NUM_2

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

# define delay_ms(ms) vTaskDelay((ms) / portTICK_PERIOD_MS)

static const char* TAG = "LED";

#define CONFIG_EXAMPLE_TYPE_DHT11
#if defined(CONFIG_EXAMPLE_TYPE_DHT11)
#define SENSOR_TYPE DHT_TYPE_DHT11
#endif

#define DATA_DHT_GPIO GPIO_NUM_17

#define TEMT6000_PIN ADC1_CHANNEL_0 // Adjust based on your wiring
#define TEMT6000_POWER_VOLTAGE 3.3


#define HC_SR04_TRIGGER GPIO_NUM_21
#define HC_SR04_ECHO GPIO_NUM_22


_Noreturn void run(void)
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    ultrasonic_sensor_t ultrasonicSensor = {
        .echo_pin = 1,
        .trigger_pin = 2,
    };

    if (ultrasonic_init(&ultrasonicSensor) == ESP_OK) {
        ESP_LOGE(TAG, "ultrasonic_init failed");
    }

    ESP_LOGE(TAG, "GPIO user started");

    float temperature, humidity;
    config_gpio();

    // Analog-to-Digital Converter (ADC)
    config_adc();

    while (1)
    {
        printf("Lux: %.1f\n", acd_read_lux(TEMT6000_PIN));

        if (dht_read_float_data(SENSOR_TYPE, DATA_DHT_GPIO, &humidity, &temperature) == ESP_OK)
            printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
        else
            printf("Could not read data from sensor\n");

        // If you read the sensor data too often, it will heat up
        // http://www.kandrsmith.org/RJS/Misc/Hygrometers/dht_sht_how_fast.html
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void config_gpio(void)
{
    gpio_set_pull_mode(DATA_DHT_GPIO, GPIO_PULLUP_ONLY);
}

void config_adc(void)
{
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(TEMT6000_PIN, ADC_ATTEN_DB_0);
}


float acd_read_lux(adc1_channel_t channel)
{
    int adc_value = adc1_get_raw(channel);
    float voltage = adc_value * (TEMT6000_POWER_VOLTAGE / 1024); // Convert ADC value to voltage
    float lux = (voltage / 10000.0) * 2000000.0; // Convert voltage to lux
    return lux;
}