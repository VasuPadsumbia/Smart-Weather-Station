##############################################################
#
# SMART WEATHER STATION - Buildroot Package Makefile
#
##############################################################

SMART_WEATHER_STATION_VERSION = 1.0.5
SMART_WEATHER_STATION_SITE = $(TOPDIR)/../app
SMART_WEATHER_STATION_SITE_METHOD = local
SMART_WEATHER_STATION_LICENSE = MIT
SMART_WEATHER_STATION_DEPENDENCIES = host-pkgconf libcurl mosquitto

# No configure step needed (if your Makefile handles everything)
define SMART_WEATHER_STATION_CONFIGURE_CMDS
	true
endef

# Build step: just call your Makefile in the app folder using Buildroot's cross compile environment
define SMART_WEATHER_STATION_BUILD_CMDS
	$(MAKE) -C $(@D) CROSS_COMPILE=aarch64-linux-gnu- BUILDROOT_SYSROOT=$(HOST_DIR)/aarch64-buildroot-linux-gnu/sysroot
endef

# Install step: copy built binaries to target root filesystem
define SMART_WEATHER_STATION_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/weather_app $(TARGET_DIR)/usr/bin/weather_app
	$(INSTALL) -m 0755 $(@D)/server_app $(TARGET_DIR)/usr/bin/server_app
	$(INSTALL) -m 0755 $(@D)/dashboard_server/server.py $(TARGET_DIR)/usr/bin/server.py
endef

$(eval $(generic-package))
