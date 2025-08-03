#include "publisher_https.h"

bool publish_http(const bme280_data_t& data, const char* url) {
    CURL *curl = curl_easy_init();
    if (!curl) {
        std::cerr << "CURL initialization failed\n";
        return false;
    }

    std::stringstream json;
    json << "{";
    json << "\"temperature\": " << data.temperature << ",";
    json << "\"pressure\": " << data.pressure << ",";
    json << "\"humidity\": " << data.humidity;
    json << "}";

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.str().c_str());
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "CURL error: " << curl_easy_strerror(res) << "\n";
        curl_easy_cleanup(curl);
        return false;
    }

    curl_easy_cleanup(curl);
    return true;
}
