#include "publisher_mqtt.h"


bool publish_mqtt(const bme280_data_t& data, const char* broker, const char* topic) {
    mosquitto_lib_init();

    mosquitto *mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
        std::cerr << "Mosquitto initialization failed\n";
        return false;
    }

    if (mosquitto_connect(mosq, broker, 1883, 60) != MOSQ_ERR_SUCCESS) {
        std::cerr << "Mosquitto connect failed\n";
        mosquitto_destroy(mosq);
        return false;
    }

    std::stringstream json;
    json << "{";
    json << "\"temperature\": " << data.temperature << ",";
    json << "\"pressure\": " << data.pressure << ",";
    json << "\"humidity\": " << data.humidity;
    json << "}";

    int ret = mosquitto_publish(mosq, NULL, topic, json.str().size(), json.str().c_str(), 0, false);
    if (ret != MOSQ_ERR_SUCCESS) {
        std::cerr << "Mosquitto publish error: " << mosquitto_strerror(ret) << "\n";
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
        return false;
    }

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return true;
}
