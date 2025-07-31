#include <curl/curl.h>
#include <sstream>
#include <iostream>
#include "bme280.h"

#pragma once


bool publish_http(const bme280_data_t& data, const char* url);
