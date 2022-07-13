#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt.h"
#include "led_strip.h"
#include "hsvcolor.h"
#include "string.h"

static const char *TAG = "sample";

#define RMT_TX_CHANNEL RMT_CHANNEL_0

#define EXAMPLE_CHASE_SPEED_MS (10)

led_strip_t *set_led_strip(uint32_t tx_gpio, uint32_t tx_channel, uint32_t led_number)
{
    rmt_config_t config = RMT_DEFAULT_CONFIG_TX(tx_gpio, tx_channel);
    config.clk_div = 2;

    ESP_ERROR_CHECK(rmt_config(&config));
    ESP_ERROR_CHECK(rmt_driver_install(config.channel, 0, 0));

    led_strip_config_t strip_config = LED_STRIP_DEFAULT_CONFIG(led_number, (led_strip_dev_t)config.channel);
    led_strip_t *strip = led_strip_new_rmt_ws2812(&strip_config);
    if (!strip) {
        ESP_LOGE(TAG, "install WS2812 driver failed");
    }
    return strip;
}

void render_led_strip_all(led_strip_t *strip, uint32_t pin_number)
{
    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;
    

    ESP_ERROR_CHECK(strip -> clear(strip, 100));

    led_strip_hsv2rgb(0, 100, 100, &red, &green, &blue);
    ESP_ERROR_CHECK(strip -> set_pixel(strip, pin_number, red, green, blue));
    ESP_ERROR_CHECK(strip -> refresh(strip, 100));

    vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
}

void app_main(void)
{
    led_strip_t strip = set_led_strip(CONFIG_EXAMPLE_RMT_TX_GPIO, RMT_TX_CHANNEL, CONFIG_EXAMPLE_STRIP_LED_NUMBER);
    render_led_strip_all(strip, 2);
}