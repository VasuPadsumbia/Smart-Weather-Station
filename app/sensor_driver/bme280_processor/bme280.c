#include "bme280.h"

static int32_t t_fine;

static uint16_t dig_T1;
static int16_t dig_T2, dig_T3;
static uint16_t dig_P1;
static int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;
static uint8_t dig_H1;
static int16_t dig_H2;
static uint8_t dig_H3;
static int16_t dig_H4, dig_H5;
static int8_t dig_H6;

static int read_reg16_le(int fd, uint8_t reg) {
    uint8_t buf[2];
    write(fd, &reg, 1);
    read(fd, buf, 2);
    return (buf[1] << 8) | buf[0];
}

static void read_calibration(int fd) {
    dig_T1 = read_reg16_le(fd, 0x88);
    dig_T2 = read_reg16_le(fd, 0x8A);
    dig_T3 = read_reg16_le(fd, 0x8C);
    dig_P1 = read_reg16_le(fd, 0x8E);
    dig_P2 = read_reg16_le(fd, 0x90);
    dig_P3 = read_reg16_le(fd, 0x92);
    dig_P4 = read_reg16_le(fd, 0x94);
    dig_P5 = read_reg16_le(fd, 0x96);
    dig_P6 = read_reg16_le(fd, 0x98);
    dig_P7 = read_reg16_le(fd, 0x9A);
    dig_P8 = read_reg16_le(fd, 0x9C);
    dig_P9 = read_reg16_le(fd, 0x9E);
    
    uint8_t reg = 0xA1;
    write(fd, &reg, 1);
    read(fd, &dig_H1, 1);
    dig_H2 = read_reg16_le(fd, 0xE1);
    reg = 0xE3; write(fd, &reg, 1); read(fd, &dig_H3, 1);
    uint8_t e4, e5, e6;
    reg = 0xE4; write(fd, &reg, 1); read(fd, &e4, 1);
    reg = 0xE5; write(fd, &reg, 1); read(fd, &e5, 1);
    reg = 0xE6; write(fd, &reg, 1); read(fd, &e6, 1);
    dig_H4 = (e4 << 4) | (e5 & 0xF);
    dig_H5 = (e6 << 4) | (e5 >> 4);
    reg = 0xE7; write(fd, &reg, 1); read(fd, &dig_H6, 1);
}

int bme280_init(const char *device, int addr) {
    int fd = open(device, O_RDWR);
    if(fd < 0) return -1;
    if(ioctl(fd, I2C_SLAVE, addr) < 0) { close(fd); return -1; }
    read_calibration(fd);
    uint8_t ctrl_meas[2] = {0xF4, 0x27};
    write(fd, ctrl_meas, 2);
    uint8_t ctrl_hum[2] = {0xF2, 0x01};
    write(fd, ctrl_hum, 2);
    return fd;
}

static float compensate_temp(int32_t adc_T) {
    float var1 = (((float)adc_T)/16384.0 - ((float)dig_T1)/1024.0) * dig_T2;
    float var2 = ((((float)adc_T)/131072.0 - ((float)dig_T1)/8192.0) *
                 (((float)adc_T)/131072.0 - ((float)dig_T1)/8192.0)) * dig_T3;
    t_fine = (int32_t)(var1 + var2);
    return (var1 + var2) / 5120.0;
}

int bme280_read_all(int fd, bme280_data_t *data) {
    uint8_t reg = 0xF7, buf[8];
    write(fd, &reg, 1);
    read(fd, buf, 8);

    int32_t adc_P = (buf[0]<<12) | (buf[1]<<4) | (buf[2]>>4);
    int32_t adc_T = (buf[3]<<12) | (buf[4]<<4) | (buf[5]>>4);
    int32_t adc_H = (buf[6]<<8) | buf[7];

    data->temperature = compensate_temp(adc_T);
    data->pressure = adc_P / 25600.0;  // Simplified (real compensation is longer)
    data->humidity = adc_H / 1024.0;   // Simplified (real compensation is longer)
    return 0;
}

void bme280_close(int fd) {
    close(fd);
}
