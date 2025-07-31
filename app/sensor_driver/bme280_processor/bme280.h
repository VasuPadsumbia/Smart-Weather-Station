#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float temperature;
    float pressure;
    float humidity;
} bme280_data_t;

int bme280_init(const char *device, int addr);
int bme280_read_all(int fd, bme280_data_t *data);
void bme280_close(int fd);

#ifdef __cplusplus
}
#endif
