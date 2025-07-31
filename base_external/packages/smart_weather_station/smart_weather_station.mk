##############################################################
#
# SMART WEATHER STATION - CMake Based Build
#
##############################################################

SMART_WEATHER_STATION_VERSION = 1.0.0
SMART_WEATHER_STATION_SITE = $(TOPDIR)/../app
SMART_WEATHER_STATION_SITE_METHOD = local
SMART_WEATHER_STATION_LICENSE = MIT
SMART_WEATHER_STATION_DEPENDENCIES = host-pkgconf

# Optional: Configure flags for CMake
SMART_WEATHER_STATION_CONF_OPTS = -DCMAKE_BUILD_TYPE=Release

# Configure step: invoke cmake with the proper target env
define SMART_WEATHER_STATION_CONFIGURE_CMDS
	(cd $(@D) && \
		$(TARGET_MAKE_ENV) $(CMAKE) \
		-DCMAKE_TOOLCHAIN_FILE=$(CMAKE_TOOLCHAIN_FILE) \
		$(SMART_WEATHER_STATION_CONF_OPTS) .)
endef

# Build step
define SMART_WEATHER_STATION_BUILD_CMDS
	$(TARGET_MAKE_ENV) $(MAKE) -C $(@D)
endef

# Install the compiled app binary to target rootfs
define SMART_WEATHER_STATION_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0755 $(@D)/app/build/weather_app $(TARGET_DIR)/usr/bin/weather_app
	$(INSTALL) -m 0755 $(@D)/app/build/server_app $(TARGET_DIR)/usr/bin/server_app
	$(INSTALL) -m 0755 $(@D)/app/dashboard_server/server.py $(TARGET_DIR)/usr/bin/server.py
endef

$(eval $(cmake-package))
