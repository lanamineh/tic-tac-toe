# tic-tac-toe

Tic-tac-toe on an ESP32-C3 micro-controller with a small LCD screen (ST7735).


## Building the project

```bash
esp_idf  # Run this every time to set up the environment
idf.py set-target esp32c3  # Only need to run this once to configure the target
idf.py build  # If you just want to build
idf.py -p /dev/ttyUSB0 flash  # If you want to build and run
idf.py monitor # To open logging console
```

## Pin connections

From top to bottom, the pins on the board are as follows:

- **Vin:** power (3.3V)
- **3.3V:** power
- **Gnd:** ground
- **SCK:** SPI clock input, connected to microcontroller pin 1
- **MISO:** Microcontroller in, serial out. This pin is connected to the SD card as the TFT display is write-only. I have chosen to connect it to ground.
- **MOSI:** Microcontroller out, serial in. Used to send data to the display, connected to microcontroller pin 0.
- **TFT_CS:** Chip select for the display. Held high as default, and then held low when data is being sent. Connected to microcontroller pin 2.
- **RESET:** If you connect this to ground it will reset the display. I have connected this to power.
- **D/C:** Set low if you are sending a command (see section 10 of the manual) to the display, and high if sending data. Connected to microcontroller pin 3.
- **CARD_CS:** Chip select for the SD card, ignored so it is connected to power.
- **LITE:** Backlight power. I have connected it to power for now (backlight on), but it can be turned off by connecting it to ground, or dimmed by pulse width modulation.

Note that the microcontroller pins have been chosen for convenience due to the breadboard layout.

See [this tutorial](https://learn.adafruit.com/1-8-tft-display?view=all) for more details. And the [manual](ST7735.pdf) for the display.
