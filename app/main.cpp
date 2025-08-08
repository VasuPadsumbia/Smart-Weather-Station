#include "bme280.h"
#include "publisher_https.h"
#include "publisher_mqtt.h"
#include <iostream>
#include <unistd.h>
#include <atomic>
#include <thread>
#include <chrono>

/*
int main() {
    int fd = bme280_init("/dev/i2c-1", 0x76);
    if(fd < 0) {
        std::cerr << "Failed to initialize BME280\n";
        return 1;
    }

    bme280_data_t data;

    if(bme280_read_all(fd, &data) != 0) {
        std::cerr << "Sensor read failed\n";
        bme280_close(fd);
        return 1;
    }

    std::cout << "Temperature: " << data.temperature << " °C\n";
    std::cout << "Pressure: " << data.pressure << " hPa\n";
    std::cout << "Humidity: " << data.humidity << " %\n";

    // MQTT broker is local for the raspberrypi
    const char* broker = "localhost";
    const char* topic = "weather/data";
    const int mqtt_port = 1883;

    if(publish_mqtt(data, broker, topic, mqtt_port))
        std::cout << "Published data to MQTT broker\n";
    else
        std::cerr << "MQTT publish failed\n";

    bme280_close(fd);
    return 0;
}*/

int main() {
    std::cout << "Smart Weather Station Application\n";
    std::cout << "Type 'start' to begin publishing, 'stop' to pause, 'exit' to quit.\n";

    // Initialize BME280 sensor
    std::cout << "Initializing BME280 sensor...\n";
    int fd = bme280_init("/dev/i2c-1", 0x76);
    if(fd < 0) {
        std::cerr << "Failed to initialize BME280\n";
        return 1;
    }

    const char* broker = "192.168.2.1";
    const char* topic = "weather/data";
    const int mqtt_port = 1883;

    std::atomic<bool> running(false);
    std::atomic<bool> exit_flag(false);

    // Thread that publishes data every 5 seconds when running is true
    std::thread publisher_thread([&]() {
        while (!exit_flag.load()) {
            if (running.load()) {
                bme280_data_t data;
                if(bme280_read_all(fd, &data) == 0) {
                    std::cout << "Temperature: " << data.temperature << " °C\n";
                    std::cout << "Pressure: " << data.pressure << " hPa\n";
                    std::cout << "Humidity: " << data.humidity << " %\n";

                    if(publish_mqtt(data, broker, topic, mqtt_port))
                        std::cout << "Published data to MQTT broker\n";
                    else
                        std::cerr << "MQTT publish failed\n";
                } else {
                    std::cerr << "Sensor read failed\n";
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    });

    std::cout << "Type 'start' to begin publishing, 'stop' to pause, 'exit' to quit.\n";
    std::string cmd;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, cmd);

        if (cmd == "start") {
            if (!running.load()) {
                running.store(true);
                std::cout << "Publishing started.\n";
            } else {
                std::cout << "Already running.\n";
            }
        } else if (cmd == "stop") {
            if (running.load()) {
                running.store(false);
                std::cout << "Publishing stopped.\n";
            } else {
                std::cout << "Already stopped.\n";
            }
        } else if (cmd == "exit") {
            std::cout << "Exiting application.\n";
            exit_flag.store(true);
            break;
        } else {
            std::cout << "Unknown command. Use 'start', 'stop', or 'exit'.\n";
        }
    }

    // Wait for publisher thread to finish
    if (publisher_thread.joinable())
        publisher_thread.join();

    bme280_close(fd);
    return 0;
}