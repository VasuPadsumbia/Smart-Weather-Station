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

1. Clone the group repository and follow the Buildroot README to build the SD‑card image.
2. Connect the BME280 sensor to the Pi Zero 2 W’s I²C pins (3.3 V, GND, SDA, SCL).
3. Flash the generated image onto a microSD card, insert it into the Pi, and power up.
4. The Python application should start automatically and begin publishing data to the configured MQTT broker.

For more information, see the wiki pages and issues linked in our GitHub Projects board.

## Authors

- **Vasu Padsumbia** – Buildroot & kernel configuration, Hardware & sensor integration, Application & cloud services

## License

This project uses open‑source libraries under their respective licenses. Our original code is released under the MIT License (see LICENSE file) unless otherwise noted.
