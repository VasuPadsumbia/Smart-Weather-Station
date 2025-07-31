#pragma once

#include "bme280.h"
#include <mosquitto.h>
#include <sstream>
#include <iostream>

bool publish_mqtt(const bme280_data_t& data, const char* broker, const char* topic);
