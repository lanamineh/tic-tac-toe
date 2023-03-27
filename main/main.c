#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"

#define PIN_NUM_MISO 9
#define PIN_NUM_MOSI 0
#define PIN_NUM_CLK 1
#define PIN_NUM_CS 2
#define PIN_NUM_DC 3

void app_main(void)
{
    ESP_LOGI("main", "Initializing bus SPI%d...", SPI2_HOST+1);
    // Set the correct IO pins
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_NUM_MISO,
        .mosi_io_num = PIN_NUM_MOSI,
        .sclk_io_num = PIN_NUM_CLK,
    };
    // Initialise the SPI bus (host)
    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_DISABLED);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("main", "SPI setup return code = %d", ret);

    // Configure the device
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10*1000*1000, 
        .mode = 3,                         
        .spics_io_num = PIN_NUM_CS,     
        .queue_size = 1,                
    };
    spi_device_handle_t device;
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &device); 
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("main", "SPI device setup return code = %d", ret);

    // Transmit some data
    const char *data = "Hello";
    spi_transaction_t t = {
	.length = 5*8,  // Length of data * num bits in a byte
	.tx_buffer = data,
    };

    while (true)
    {
	ret=spi_device_polling_transmit(device, &t);  //Transmit!
	ESP_ERROR_CHECK(ret);
	ESP_LOGI("main", "Transmit return code = %d", ret);
	vTaskDelay(1 / portTICK_PERIOD_MS);
    }
}
