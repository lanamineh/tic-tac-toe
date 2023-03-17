# tic-tac-toe

Tic-tac-toe on an ESP32-C3 micro-controller with a small LCD screen (ST7735).


## Building the project

```bash
esp_idf  # Run this every time to set up the environment
idf.py set-target esp32c3  # Only need to run this once to configure the target
idf.py build  # If you just want to build
idf.py -p /dev/ttyUSB0 flash  # If you want to build and run
```
