#
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk

PKG_NAME:=ufsd-85

PKG_VERSION:=8.5.0
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(KERNEL_BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
KERNEL_VER:=$(shell echo $(LINUX_DIR)|grep -oP "(?<=/linux-)\d\.\d+")

include $(INCLUDE_DIR)/package.mk


define KernelPackage/ufsd-85
  SUBMENU:=Filesystems
  TITLE:=Paragon ufsd 8.5
  DEPENDS:=+kmod-nls-base +kmod-nls-utf8 @powerpc
  #@TARGET_ar71xx
  FILES:=$(PKG_BUILD_DIR)/ufsd.ko 
  AUTOLOAD:=$(call AutoLoad,50,ufsd)
endef

define KernelPackage/ufsd-85/description
Paragon ufsd is a fs driverof ntfs
endef

#define KernelPackage/raeth/config
#	source "$(SOURCE)/config.in"
#endef

SOURCE_DIR:=$(PKG_BUILD_DIR)
export SOURCE_DIR

MAKEOPTS:= -C $(LINUX_DIR) \
                ARCH="$(LINUX_KARCH)" \
                CROSS_COMPILE="$(TARGET_CROSS)" \
                M="$(PKG_BUILD_DIR)" \
		CONFIG_UFSD_FS=m \
		modules

define Build/Prepare
	$(call Build/Prepare/Default)
	cp -r src-$(KERNEL_VER)/. $(PKG_BUILD_DIR)
	$(CP) obj/*.o_shipped $(PKG_BUILD_DIR)/
endef


define Build/Compile
	$(MAKE) $(MAKEOPTS)
endef

define KernelPackage/ufsd-85/install
endef



$(eval $(call KernelPackage,ufsd-85))
