#include "container.h"

#include <cJSON.h>
#include <wifi.h>

#include <memory>
#include <mqtt.hh>

#include "dht.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"
#include "mqtt_client.h"
#include "nvs_flash.h"

#define BLINK_GPIO GPIO_NUM_2
#define HC_SR04_ECHO GPIO_NUM_16
#define HC_SR04_TRIGGER GPIO_NUM_17
#define DATA_DHT_GPIO GPIO_NUM_4
#define TEMT6000_PIN ADC1_CHANNEL_0
#define MAX_DISTANCE_CM 400
#define DHT_GPIO_DATA GPIO_NUM_4

const auto LIGHT_TAG = "LIGHT sensor";

SensorContainer::SensorContainer() {
    init_wifi_with_nvs();

    dht11 = std::make_unique<DHT11>(DHT_GPIO_DATA);
    hc_sr04 = std::make_unique<Ultrasonic>(HC_SR04_TRIGGER, HC_SR04_ECHO);
    mqtt_client = std::make_unique<MQTTClient>("esp32/plant_monitoring/");
    temt6000 = std::make_unique<Temt6000>(TEMT6000_PIN);
}

void SensorContainer::read_gpio_sensors(float &temperature, float &humidity, float &distance) const {
    if (const auto dht_esp_err = dht11->read_data(temperature, humidity); dht_esp_err != ESP_OK) {
        ESP_LOGE(DHT_TAG, "DHT read error: %s", esp_err_to_name(dht_esp_err));
    }

    if (const auto ultrasonic_esp_err = hc_sr04->read_data(distance, temperature); ultrasonic_esp_err != ESP_OK) {
        ESP_LOGE(DHT_TAG, "HC-SR-04 read error: %s", esp_err_to_name(ultrasonic_esp_err));
    };
}

_Noreturn void run()
{
    const SensorContainer container;
    float temperature, humidity, distance, lux;

    while (true) {
        // collect metrics from sensors - order is important
        if (const auto dht_err = container.dht11->read_data(temperature, humidity); dht_err != ESP_OK) {
            ESP_LOGE(DHT_TAG, "DHT read error: %s", esp_err_to_name(dht_err));
        }

        if (const auto temt_err = container.temt6000->read_data(lux); temt_err != ESP_OK) {
            ESP_LOGE(DHT_TAG, "DHT read error: %s", esp_err_to_name(temt_err));
        }

        if (const auto ultrasonic_err = container.hc_sr04->read_data(distance, temperature); ultrasonic_err != ESP_OK) {
            ESP_LOGE(DHT_TAG, "HC-SR-04 read error: %s", esp_err_to_name(ultrasonic_err));
        };

        printf("Humidity: %.1f%% Temp: %.1fC Distance: %.4f m Lux: %.1f\n", humidity, temperature, distance, lux);

        // marshal metrics and send to rhe mqtt-broker
        const auto metrics = marshal_metrics_to_json(temperature, humidity, lux, distance);
        container.mqtt_client->publish("metrics", metrics);

        // If you read the sensor data too often, it will heat up
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
}

std::string marshal_metrics_to_json(const float temperature, const float humidity,
                                 const float lux, const float distance)
{
    cJSON *root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "temperature", temperature);
    cJSON_AddNumberToObject(root, "humidity", humidity);
    cJSON_AddNumberToObject(root, "lux", lux);
    cJSON_AddNumberToObject(root, "distance", distance);
    return cJSON_Print(root);
}

void SensorContainer::init_wifi_with_nvs() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(WIFI_LOG_TAG, "ESP_WIFI_MODE_STA");
    wifi_init_station_mode();
}