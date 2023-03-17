#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED1 GPIO_NUM_6
#define LED2 GPIO_NUM_7

void set_led(bool state)
{
    gpio_set_level(LED1, state);
    gpio_set_level(LED2, state);
}

void app_main(void)
{
    gpio_config_t io_conf =
	{
	    .pin_bit_mask = ((1ULL << LED1) | (1ULL << LED2)),
	    .mode = GPIO_MODE_OUTPUT,
	    .pull_up_en = 0,
	    .pull_down_en = 1,
	    .intr_type = GPIO_INTR_DISABLE
	};
    //configure GPIO with the given settings
    gpio_config(&io_conf);
    
    bool state = false;
    while(1)
    {
    	set_led(false);
    	vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
