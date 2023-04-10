#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "commands.h"

#define PIN_MISO 9
#define PIN_MOSI 10
#define PIN_CLK 1
#define PIN_CS 2
#define PIN_DC 3

/* spi_device_handle_t initialise_spi() */
/* { */
/*     ESP_LOGI("main", "Initializing bus SPI%d...", SPI2_HOST+1); */
/*     // Set the correct IO pins */
/*     spi_bus_config_t buscfg = { */
/*         .miso_io_num = PIN_MISO, */
/*         .mosi_io_num = PIN_MOSI, */
/*         .sclk_io_num = PIN_CLK, */
/*     }; */
/*     // Initialise the SPI bus (host) */
/*     esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_DISABLED); */
/*     ESP_ERROR_CHECK(ret); */
/*     ESP_LOGI("main", "SPI setup return code = %d", ret); */

/*     // Configure the device */
/*     spi_device_interface_config_t devcfg = { */
/*         .clock_speed_hz = 10*1000*1000,  */
/*         .mode = 0,                          */
/*         .spics_io_num = PIN_CS,      */
/*         .queue_size = 1,                 */
/*     }; */
/*     spi_device_handle_t device; */
/*     ret = spi_bus_add_device(SPI2_HOST, &devcfg, &device);  */
/*     ESP_ERROR_CHECK(ret); */
/*     ESP_LOGI("main", "SPI device setup return code = %d", ret); */

/*     return device; */
/* } */

void write_command(spi_device_handle_t device, uint8_t cmd)
{
    gpio_set_level(PIN_DC, 0);
    spi_transaction_t t = {
	//.flags = SPI_TRANS_USE_TXDATA,
	.length = 8,
	//.tx_data = &cmd,
	.tx_buffer = &cmd
    };
    spi_device_polling_transmit(device, &t); 
}

void delay_ms(unsigned time)
{
    vTaskDelay(time / portTICK_PERIOD_MS);
}

void initialise_display(spi_device_handle_t device)
{
    write_command(device, SWRESET);
    delay_ms(120);

    write_command(device, SLPOUT);
    delay_ms(120);

    write_command(device, DISPON);
}

void write_data(spi_device_handle_t device, uint8_t * data, size_t len)
{
    //gpio_set_level(PIN_DC, 1);
    spi_transaction_t t = {
	.length = len*8,
	.tx_buffer = data,
    };
    spi_device_polling_transmit(device, &t);
    //ESP_LOGI("main", "DC pin... %d", gpio_get_level(PIN_DC));
}


void app_main(void)
{
    ESP_LOGI("main", "Initializing bus SPI%d...", SPI2_HOST+1);
    // Set the correct IO pins
    spi_bus_config_t buscfg = {
        .miso_io_num = PIN_MISO,
        .mosi_io_num = PIN_MOSI,
        .sclk_io_num = PIN_CLK,
    };
    // Initialise the SPI bus (host)
    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_DISABLED);
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("main", "SPI setup return code = %d", ret);

    // Configure the device
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = 10*1000*1000, 
        .mode = 0,                         
        .spics_io_num = PIN_CS,     
        .queue_size = 1,                
    };
    spi_device_handle_t device;
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &device); 
    ESP_ERROR_CHECK(ret);
    ESP_LOGI("main", "SPI device setup return code = %d", ret);
    
    // Set up the D/C pin as a general input output pin
    gpio_config_t io_conf = {
	.pin_bit_mask = (1ULL << PIN_DC),
	.mode = GPIO_MODE_OUTPUT,
	.pull_up_en = 0,
	.pull_down_en = 1,
	.intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    //spi_device_handle_t device = initialise_spi();
    initialise_display(device);

    /* write_command(device, RASET); */
    /* // IMPORTANT: This doesn't work before display is on */
    /* // Try to get one row */
    /* uint8_t y_vals[] = {0x00, 50, 0x00, 52}; */
    /* write_data(device, y_vals); */
    write_command(device, RAMWR);
    // Write pixels
    gpio_set_level(PIN_DC, 1);
    delay_ms(10);
    ESP_LOGI("main", "DC pin... %d", gpio_get_level(PIN_DC));

    uint8_t color[] = {0x00, 0xff, 0x00};
    for (int i = 0; i < 100; i++) {
	write_data(device, color, sizeof color);
    }


    
    // To display
    // CASET can go from 2 to 129
    // RASET can go from 1 to 128 
    
    
    while (true)
    {
	vTaskDelay(120 / portTICK_PERIOD_MS);
	/* ret=spi_device_polling_transmit(device, &t);  //Transmit! */
	/* ESP_ERROR_CHECK(ret); */
	/* ESP_LOGI("main", "Transmit return code = %d", ret); */
	/* vTaskDelay(1 / portTICK_PERIOD_MS); */
    }
}
