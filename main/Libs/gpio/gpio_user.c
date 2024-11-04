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

# define BLINK_GPIO GPIO_NUM_2

#define LOG_LOCAL_LEVEL ESP_LOG_VERBOSE

# define delay_ms(ms) vTaskDelay((ms) / portTICK_PERIOD_MS)

static const char* TAG = "LED";

_Noreturn void gpio_user(void)
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    ESP_LOGE(TAG, "GPIO user started");
    int i = 0;

    while (1)
    {
        gpio_set_level(BLINK_GPIO, 0);
        delay_ms(1000);
        gpio_set_level(BLINK_GPIO, 1);
        delay_ms(1000);
        ESP_LOGI(TAG, "%d", i++);
    }
}
