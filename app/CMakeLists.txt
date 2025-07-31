cmake_minimum_required(VERSION 3.10)
project(SmartWeatherStation)

set(CMAKE_CXX_STANDARD 11)

find_package(CURL REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)

add_executable(weather_app
    main.cpp
    sensor_driver/bme280_processor/bme280.c
    sensor_driver/cpp_https_publisher/publisher_https.cpp
    sensor_driver/cpp_mqtt_publisher/publisher_mqtt.cpp
)
add_executable(server_app
    dashboard_server/multi_client_server.cpp
)

target_link_libraries(weather_app
    CURL::libcurl
    ${MOSQUITTO_LIBRARIES}
)
target_link_libraries(server_app pthread)

# Include directories (the fix)
target_include_directories(weather_app PRIVATE 
    ${CMAKE_SOURCE_DIR}/sensor_driver/bme280_processor
    sensor_driver/cpp_https_publisher
    sensor_driver/cpp_mqtt_publisher
    ${MOSQUITTO_INCLUDE_DIRS}
)

