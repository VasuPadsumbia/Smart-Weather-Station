# Makefile for Smart Weather Station apps

ifeq ($(origin CROSS_COMPILE), undefined)
CC = gcc
CXX = g++
else
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
endif

BUILDROOT_SYSROOT ?= /home/vasu/AELD/Smart-Weather-Station/buildroot/output/host/aarch64-buildroot-linux-gnu/sysroot

CFLAGS ?= -Wall -std=c11
CFLAGS += -I./sensor_driver/bme280_processor \
          -I./sensor_driver/cpp_https_publisher \
          -I./sensor_driver/cpp_mqtt_publisher \
		  --sysroot=$(BUILDROOT_SYSROOT) \
          -I$(BUILDROOT_SYSROOT)/usr/include

CXXFLAGS ?= -Wall -std=c++11
CXXFLAGS += -I./sensor_driver/bme280_processor \
           -I./sensor_driver/cpp_https_publisher \
           -I./sensor_driver/cpp_mqtt_publisher \
           --sysroot=$(BUILDROOT_SYSROOT) \
           -I$(BUILDROOT_SYSROOT)/usr/include

LDFLAGS ?= --sysroot=$(BUILDROOT_SYSROOT) -L$(BUILDROOT_SYSROOT)/usr/lib -lcurl -lmosquitto -lpthread

# Sources
WEATHER_SRCS = main.cpp \
               sensor_driver/bme280_processor/bme280.c \
               sensor_driver/cpp_https_publisher/publisher_https.cpp \
               sensor_driver/cpp_mqtt_publisher/publisher_mqtt.cpp

SERVER_SRCS = dashboard_server/multi_client_server.cpp

# Objects
WEATHER_OBJS = $(WEATHER_SRCS:.cpp=.o)
WEATHER_OBJS := $(WEATHER_OBJS:.c=.o)

SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)

.PHONY: all clean

all: weather_app server_app

weather_app: $(WEATHER_OBJS)
	@echo "CXX = $(CXX)"
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

server_app: $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lpthread -lmosquitto -lcurl $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f weather_app server_app $(WEATHER_OBJS) $(SERVER_OBJS)
