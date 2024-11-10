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
#include "ultrasonic.h"

#define BLINK_GPIO GPIO_NUM_2
#define HC_SR04_ECHO GPIO_NUM_16
#define HC_SR04_TRIGGER GPIO_NUM_17
#define DATA_DHT_GPIO GPIO_NUM_4
#define TEMT6000_PIN ADC1_CHANNEL_0
#define TEMT6000_POWER_VOLTAGE 3.3
#define MAX_DISTANCE_CM 400
#define SENSOR_TYPE DHT_TYPE_DHT11
#define TEMT6000_POWER_VOLTAGE 3.3

const auto DHT_TAG = "DHT sensor";
const auto ULTRASONIC_TAG = "ULTRASONIC sensor";
const auto LIGHT_TAG = "LIGHT sensor";

SensorContainer::SensorContainer() {
    // Init DHT11
    config_dht11_gpio(DATA_DHT_GPIO);

    // Initialize HC_SR04
    ultrasonic_sensor = {
        .trigger_pin = HC_SR04_TRIGGER,
        .echo_pin = HC_SR04_ECHO,
    };

    if (const auto ultrasonicInitErr = ultrasonic_init(&ultrasonic_sensor);
        ultrasonicInitErr != ESP_OK) {
        ESP_LOGE(ULTRASONIC_TAG, "ultrasonic_init failed %s", esp_err_to_name(ultrasonicInitErr));
    }

    // Create mqtt_client
    mqtt_client = std::make_unique<MQTTClient>("esp32/plant_monitoring/");

    // Init NVS
    init_wifi_with_nvs();
}

void SensorContainer::read_gpio_sensors(float &temperature, float &humidity, float &distance) const {
    // Read DHT11 data
    if (const auto dhtEspErr = dht_read_float_data(SENSOR_TYPE, DATA_DHT_GPIO, &humidity, &temperature); dhtEspErr != ESP_OK) {
        ESP_LOGE(DHT_TAG, "DHT read error: %s", esp_err_to_name(dhtEspErr));
    }

    // Read Ultrasonic sensor data
    if (const auto res = ultrasonic_measure_temp_compensated(&ultrasonic_sensor, MAX_DISTANCE_CM, &distance, temperature)) {
        ESP_LOGE(DHT_TAG, "HC-SR-04 read error: %s", esp_err_to_name(res));
    };
}

float SensorContainer::read_lux() {
    return read_lux_from_acd(TEMT6000_PIN);
}

void SensorContainer::config_dht11_gpio(gpio_num_t gpio_num) {
    gpio_set_pull_mode(gpio_num, GPIO_PULLUP_ONLY);
}

void SensorContainer::config_adc() {
    adc1_config_width(ADC_WIDTH_BIT_10);
    adc1_config_channel_atten(TEMT6000_PIN, ADC_ATTEN_DB_0);
}

float SensorContainer::read_lux_from_acd(const adc1_channel_t channel) {
    const int adcValue = adc1_get_raw(channel);
    const float voltage = adcValue * (TEMT6000_POWER_VOLTAGE / 1024); // Convert ADC value to voltage
    const float lux = (voltage / 10000.0) * 2000000.0; // Convert voltage to lux
    return lux;
}

_Noreturn void run()
{
    const SensorContainer container;
    float temperature, humidity, distance;

    while (true) {
        const float lux = SensorContainer::read_lux();
        printf("Lux: %.1f\n", lux);

        container.read_gpio_sensors(temperature, humidity, distance);

        printf("Humidity: %.1f%% Temp: %.1fC Distance: %.4f m\n", humidity, temperature, distance);
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