#include "bme280.h"
#include "publisher_https.h"
#include "publisher_mqtt.h"
#include <iostream>
#include <unistd.h>

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

    if(publish_http(data, "https://example.com/weather"))
        std::cout << "Published data to HTTP endpoint\n";
    else
        std::cerr << "HTTP publish failed\n";

    if(publish_mqtt(data, "mqtt.example.com", "weather/data"))
        std::cout << "Published data to MQTT broker\n";
    else
        std::cerr << "MQTT publish failed\n";

    bme280_close(fd);
    return 0;
}
