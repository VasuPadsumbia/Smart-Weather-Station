# Smart Weather Station – Final Project

This repository contains the individual submission for the Embedded Linux final project. The goal of this project is to build a low‑power smart weather station using a Raspberry Pi Zero 2 W, custom software, and a tailored Buildroot Linux system.

## Project Overview

Our station reads data from a BME280 sensor (temperature, humidity, pressure) and publishes the measurements to a cloud dashboard over Wi‑Fi. The device uses a Buildroot‑based Linux image customized for the Raspberry Pi Zero 2 W and includes Python scripts for sensor interfacing, an MQTT client for data transmission, and optional local web configuration through a Flask API.

The detailed project plan — including goals, system architecture, hardware specifications, open‑source components, sprint schedule, and team roles — is documented in our shared Project Overview wiki page: [👉 Project Overview](https://github.com/cu-ecen-aeld/final-project-VasuPadsumbia/wiki)

Please refer to the wiki for design diagrams and the full schedule.

## Repository Structure

- `/buildroot/` — Buildroot configuration and board files (in group repository).
- `/app/` — User‑space application code (Python) for sensor reading, MQTT publishing, and web API.
- `/docs/` — Additional documentation or diagrams if needed.

(Individual repositories may contain a subset of the above, depending on personal contributions.)

## Getting Started

To build and run the project:

1. Clone the group repository and run `./build.sh`.
    This will set up the Buildroot environment and download the necessary dependencies.
2. Before the flash and build update the config_zero2w_64.txt from buildroot/board/raspberrypizero2w-64 with the following in order to connect with wifi and enable i2c properly
    ```
    # Enable Wi-Fi and I²C
        device_tree=bcm2710-rpi-zero-2-w.dtb # use the correct device tree for Pi Zero 2 W
        dtparam=act_led_trigger=heartbeat # to enable heartbeat LED 
        dtparam=i2c_arm=on # to enable I²C
        dtparam=spi=on # to enable SPI
    ```
3. If need any additional package got to buildroot directory and run `make menuconfig` to add any additional packages.
4. Run ./save_config.sh to save the configuration and then run ./build.sh to build the image.
5. This will download Buildroot, apply the necessary patches, and build the custom Linux image for the Raspberry Pi Zero 2 W.
6. The output image will be located in `buildroot/output/images/`.
7. Flash the generated image onto a microSD card using a tool like `dd` or Balena Etcher.
8. Insert the microSD card into the Raspberry Pi Zero 2 W and power it up.
9. The Python application should start automatically, reading sensor data and publishing it to the configured MQTT broker.

For more information, see the wiki pages and issues linked in our GitHub Projects board.

## Authors

- **Vasu Padsumbia** – Buildroot & kernel configuration, Hardware & sensor integration, Application & cloud services

## License

This project uses open‑source libraries under their respective licenses. Our original code is released under the MIT License (see LICENSE file) unless otherwise noted.
