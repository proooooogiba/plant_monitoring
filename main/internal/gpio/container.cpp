#include <memory>
#include <mqtt.hh>
#include <wifi.h>

#include "esp_log.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "nvs_flash.h"
#include "mqtt_client.h"
#include "dht.h"
#include "ultrasonic.h"
#include "container.h"

#define BLINK_GPIO GPIO_NUM_2
#define HC_SR04_ECHO GPIO_NUM_15
#define HC_SR04_TRIGGER GPIO_NUM_14
#define DATA_DHT_GPIO GPIO_NUM_4
#define TEMT6000_PIN ADC1_CHANNEL_0
#define TEMT6000_POWER_VOLTAGE 3.3
#define MAX_DISTANCE 4.0
#define SENSOR_TYPE DHT_TYPE_DHT11
#define TEMT6000_POWER_VOLTAGE 3.3
#define MAX_DISTANCE 4.0
#define EXAMPLE_ADC_ATTEN ADC_ATTEN_DB_12
#define EXAMPLE_ADC1_CHAN0 ADC_CHANNEL_4
#define EXAMPLE_ADC1_CHAN1 ADC_CHANNEL_5


const auto DHT_TAG = "DHT sensor";
const auto ULTRASONIC_TAG = "ULTRASONIC sensor";
const auto LIGHT_TAG = "LIGHT sensor";

SensorContainer::SensorContainer() {
    // Initialize DHT11
    configGpio();

    // Initialize HC_SR04
    ultrasonicSensor = {
        .trigger_pin = HC_SR04_TRIGGER,
        .echo_pin = HC_SR04_ECHO,
    };

    auto ultrasonicInitErr = ultrasonic_init(&ultrasonicSensor);
    if (ultrasonicInitErr != ESP_OK) {
          ESP_LOGE(ULTRASONIC_TAG, "ultrasonic_init failed %s", esp_err_to_name(ultrasonicInitErr));
    }
}

void SensorContainer::readGpioSensors(float &temperature, float &humidity, float &distance)
{
    // Read DHT11 data
    if (auto dhtEspErr = dht_read_float_data(SENSOR_TYPE, DATA_DHT_GPIO, &humidity, &temperature); dhtEspErr != ESP_OK) {
        ESP_LOGE(DHT_TAG, "DHT read error: %s", esp_err_to_name(dhtEspErr));
    }

    // Read Ultrasonic sensor data
    if (auto ultrasonicEspErr = ultrasonic_measure(&ultrasonicSensor, MAX_DISTANCE, &distance); ultrasonicEspErr != ESP_OK) {
        ESP_LOGE(ULTRASONIC_TAG, "Ultrasonic measure error: %s", esp_err_to_name(ultrasonicEspErr));
    }
}

float SensorContainer::readLux() {
    return readLuxFromAcd(TEMT6000_PIN);
}

void SensorContainer::configGpio() {
    gpio_set_pull_mode(DATA_DHT_GPIO, GPIO_PULLUP_ONLY);
}

void SensorContainer::configAdc() {
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(TEMT6000_PIN, ADC_ATTEN_DB_0);
}

float SensorContainer::readLuxFromAcd(const adc1_channel_t channel) {
    int adcValue = adc1_get_raw(channel);
    float voltage = adcValue * (TEMT6000_POWER_VOLTAGE / 1024); // Convert ADC value to voltage
    float lux = (voltage / 10000.0) * 2000000.0; // Convert voltage to lux
    return lux;
}



_Noreturn void run() {
    // Initialize NVS
    initWifiWithNvs();

    auto mqtt_client = std::make_unique<MQTTClient>("esp32/plant_monitoring/");
    SensorContainer container;
    float temperature, humidity, distance;

    while (true) {
        printf("Lux: %.1f\n", container.readLux());

        container.readGpioSensors(temperature, humidity, distance);

        printf("Humidity: %.1f%% Temp: %.1fC\n", humidity, temperature);
        printf("Distance: %.4f m\n", distance);
        mqtt_client->publish("esp32-plant-monitoring-metrics", "ya-ya");

        // If you read the sensor data too often, it will heat up
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void initWifiWithNvs()
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(WIFI_LOG_TAG, "ESP_WIFI_MODE_STA");
    wifi_init_station_mode();
}