#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"

#define PIN_NUM_MISO 9
#define PIN_NUM_MOSI 10
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
        .mode = 0,                         
        .spics_io_num = PIN_NUM_CS,     
        .queue_size = 1,                
    };
    spi_device_handle_t device;
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &device); 
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("main", "SPI device setup return code = %d", ret);

    // Set up the D/C pin as a general input output pin
    gpio_config_t io_conf = {
	.pin_bit_mask = (1ULL << PIN_NUM_DC),
	.mode = GPIO_MODE_OUTPUT,
	.pull_up_en = 0,
	.pull_down_en = 1,
	.intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);    
    gpio_set_level(PIN_NUM_DC, 0);  // Only transmit commands
    
    // Transmit some data
    uint8_t data = 0x01; // Software reset
    //const char * data = "Hi";
    spi_transaction_t t = {
	.length = 8,  // Length of data * num bits in a byte
	.tx_buffer = &data,
    };
    spi_device_polling_transmit(device, &t); 
    vTaskDelay(120 / portTICK_PERIOD_MS);

    data = 0x11; // Sleep out
    spi_device_polling_transmit(device, &t);
    vTaskDelay(120 / portTICK_PERIOD_MS);

    // Set 18-bit pixel mode
    data = 0x3a;
    spi_device_polling_transmit(device, &t);
    gpio_set_level(PIN_NUM_DC, 1);
    data = 0x06;
    spi_device_polling_transmit(device, &t);

    gpio_set_level(PIN_NUM_DC, 0);
    data = 0x29; // Display on
    spi_device_polling_transmit(device, &t); 

    // Try to get one row
    /* data = 0x2b;  // Row address set */
    /* spi_device_polling_transmit(device, &t); */
    /* gpio_set_level(PIN_NUM_DC, 1); */
    /* uint8_t y_vals[] = {0x00, 0x00, 0x00, 0x04}; */
    /* t.length = 4*8; */
    /* t.tx_buffer = y_vals; */
    /* spi_device_polling_transmit(device, &t); */
    // Enable write to ram
    gpio_set_level(PIN_NUM_DC, 0);
    data = 0x2c;
    /* t.length = 8; */
    /* t.tx_buffer = &data; */
    spi_device_polling_transmit(device, &t);
    // Write pixels
    gpio_set_level(PIN_NUM_DC, 1);
    uint8_t color[] = {0x00, 0x00, 0xff};
    t.length = 3*8;
    t.tx_buffer = color;
    for (int i = 0; i < 128*160; i++) {
	spi_device_polling_transmit(device, &t);
    }

    
    
    while (true)
    {
	vTaskDelay(120 / portTICK_PERIOD_MS);
	/* ret=spi_device_polling_transmit(device, &t);  //Transmit! */
	/* ESP_ERROR_CHECK(ret); */
	/* ESP_LOGI("main", "Transmit return code = %d", ret); */
	/* vTaskDelay(1 / portTICK_PERIOD_MS); */
    }
}
