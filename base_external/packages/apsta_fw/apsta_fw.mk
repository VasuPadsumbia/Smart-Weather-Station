################################################################################
#  APSTA_FW – fetch AP-STA blob from the Git repo and install it
################################################################################

APSTA_FW_SITE          = https://github.com/RPi-Distro/firmware-nonfree.git
APSTA_FW_SITE_METHOD   = git
# Freeze to a commit that is known to contain the file
APSTA_FW_GIT_SUBMODULES = NO
APSTA_FW_VERSION       = bookworm 

APSTA_FW_LICENSE       = Proprietary
APSTA_FW_REDISTRIBUTE  = NO      # Buildroot won’t try to mirror it

define APSTA_FW_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0644 $(@D)/brcm80211/brcm/brcmfmac43436-sdio.apsta.bin \
	    $(TARGET_DIR)/lib/firmware/brcm/
	ln -sf brcmfmac43436-sdio.apsta.bin \
	    $(TARGET_DIR)/lib/firmware/brcm/brcmfmac43436-sdio.bin
endef

$(eval $(generic-package))
