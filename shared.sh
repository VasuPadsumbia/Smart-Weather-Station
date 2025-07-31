#!/bin/sh
# Shared definitions for buildroot scripts

# The defconfig from the buildroot directory used for Pi Zero 2 W builds
PI_ZERO2W_DEFCONFIG=configs/raspberrypizero2w_64_defconfig

# The place to store customizations to the Pi Zero 2 W configuration
MODIFIED_PI_ZERO2W_DEFCONFIG=base_external/configs/raspberrypizero2w_custom_defconfig

# Default/fallback defconfig from the buildroot directory
AESD_DEFAULT_DEFCONFIG=${PI_ZERO2W_DEFCONFIG}
AESD_MODIFIED_DEFCONFIG=${MODIFIED_PI_ZERO2W_DEFCONFIG}
AESD_MODIFIED_DEFCONFIG_REL_BUILDROOT=../${AESD_MODIFIED_DEFCONFIG}
