#include "bme280.h"

static float t_fine_f;

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
    if (write(fd, &reg, 1) != 1) return -1;
    if (read(fd, buf, 2) != 2) return -1;
    return (buf[1] << 8) | buf[0];
}

static uint8_t read_reg8(int fd, uint8_t reg) {
    uint8_t buf;
    if (write(fd, &reg, 1) != 1) return 0xFF; // Error
    if (read(fd, &buf, 1) != 1) return 0xFF; // Error
    return buf;
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
    
    dig_H1 = read_reg8(fd, 0xA1);
    dig_H2 = read_reg16_le(fd, 0xE1);
    dig_H3 = read_reg8(fd, 0xE3);

    uint8_t e4, e5, e6;
    e4 = read_reg8(fd, 0xE4);
    e5 = read_reg8(fd, 0xE5);
    e6 = read_reg8(fd, 0xE6);

    int16_t h4 = (int16_t)((e4 << 4) | (e5 & 0x0F)); // e5 is 4 bits
    int16_t h5 = (int16_t)((e6 << 4) | (e5 >> 4)); // e5 is 4 bits
    if (h4 & 0x08000) h4 |= 0xF000; // Sign extend
    if (h5 & 0x08000) h5 |= 0xF000; // Sign extend
    dig_H4 = h4;
    dig_H5 = h5;

    dig_H6 = (int8_t)read_reg8(fd, 0xE7);
}

int bme280_init(const char *device, int addr) {
    int fd = open(device, O_RDWR);
    if(fd < 0) return -1;
    if(ioctl(fd, I2C_SLAVE, addr) < 0) { close(fd); return -1; }
    read_calibration(fd);
    // !!! Write ctrl_hum first, then ctrl_meas !!! Bosch says so
    uint8_t ctrl_hum[2] = {0xF2, 0x01};
    write(fd, ctrl_hum, 2);

    uint8_t ctrl_meas[2] = {0xF4, 0x27};
    write(fd, ctrl_meas, 2);
    
    return fd;
}

static float compensate_temp(int32_t adc_T) {
    float var1 = (((float)adc_T)/16384.0f - ((float)dig_T1)/1024.0f) * (float)dig_T2;
    float var2 = ((((float)adc_T)/131072.0f - ((float)dig_T1)/8192.0f) *
                 (((float)adc_T)/131072.0f - ((float)dig_T1)/8192.0f)) * (float)dig_T3;
    t_fine_f = var1 + var2;
    return (var1 + var2) / 5120.0f; // Temperature in Celsius
}

static float compensate_pressure(int32_t adc_P) {
    float var1 = (t_fine_f / 2.0f) - 64000.0f;
    float var2 = var1 * var1 * ((float)dig_P6) / 32768.0f;
    var2 = var2 + var1 * ((float)dig_P5) * 2.0f;
    var2 = (var2 / 4.0f) + (((float)dig_P4) * 65536.0f);
    var1 = (((float)dig_P3) * var1 * var1 / 524288.0f + ((float)dig_P2) * var1) / 524288.0f;
    var1 = (1.0f + var1 / 32768.0f) * ((float)dig_P1);
    if (var1 == 0.0f) return 0.0f; // Avoid division by zero
    float p = 1048576.0f - (float)adc_P;
    p = (p - (var2 / 4096.0f)) * 6250.0f / var1;
    var1 = ((float)dig_P9) * p * p / 2147483648.0f;
    var2 = p * ((float)dig_P8) / 32768.0f;
    p = p + (var1 + var2 + ((float)dig_P7)) / 16.0f;
    return p / 100.0f; // Pressure in hPa
}

static float compensate_humidity(int32_t adc_H) {
    // Bosch BME280 humidity compensation formula
    float var_H = t_fine_f - 76800.0f;
    float x = ((float)adc_H - (((float)dig_H4) * 64.0f + ((float)dig_H5) / 16384.0f * var_H));
    float y = ((float)dig_H2) / 65536.0f * (1.0f + ((float)dig_H6) / 67108864.0f * var_H * (1.0f + ((float)dig_H3) / 67108864.0f * var_H));
    var_H = x * y;
    var_H = var_H * (1.0f - ((float)dig_H1) * var_H / 524288.0f);
    if (var_H > 100.0f) var_H = 100.0f;
    else if (var_H < 0.0f) var_H = 0.0f;
    return var_H; // Humidity in %RH
}

int bme280_read_all(int fd, bme280_data_t *data) {
    uint8_t reg = 0xF7, buf[8];
    if (write(fd, &reg, 1) != 1) return -1;
    if (read(fd, buf, 8) != 8) return -1;

    int32_t adc_P = (buf[0]<<12) | (buf[1]<<4) | (buf[2]>>4);
    int32_t adc_T = (buf[3]<<12) | (buf[4]<<4) | (buf[5]>>4);
    int32_t adc_H = (buf[6]<<8) | buf[7];

    data->temperature = compensate_temp(adc_T);
    data->pressure = compensate_pressure(adc_P);
    data->humidity = compensate_humidity(adc_H);
    return 0;
}

void bme280_close(int fd) {
    close(fd);
}
