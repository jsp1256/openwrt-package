include $(TOPDIR)/rules.mk
 
PKG_NAME:=qt
PKG_VERSION:=5.14.1
PKG_RELEASE:=1
 
PKG_BUILD_DIR:=$(BUILD_DIR)/qt-everywhere-src-$(PKG_VERSION)
PKG_SOURCE:=qt-everywhere-src-$(PKG_VERSION).tar.xz

include $(INCLUDE_DIR)/package.mk

#CONFIG_BUILD_NLS:=y
#include $(INCLUDE_DIR)/nls.mk
MAX_CPU:=`cat /proc/cpuinfo | grep processor | wc -l`



define Package/qt/default
  SECTION:=qt librarys
  CATEGORY:=Package
  SUBMENU:=QtLib
  DEPENDS:=+libopenssl +zlib +libatomic
endef

define Package/qt-core
$(call Package/qt/default)
  TITLE:=Qt Core Library
  DEPENDS+=+icu
endef

define Package/qt-core/install
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/lib/libQt5Core.so* $(1)/usr/lib/
endef

define Package/qt-network
$(call Package/qt/default)
  TITLE:=Qt Network Library
  DEPENDS+=+qt-core
endef

define Package/qt-network/install
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/lib/libQt5Network.so* $(1)/usr/lib/
endef

define Package/qt-xml
$(call Package/qt/default)
  TITLE:=Qt Xml Library
   DEPENDS+=+qt-core qt-network
endef

define Package/qt-xml/install
	$(INSTALL_DIR) $(1)/usr/lib/
	$(CP) $(PKG_INSTALL_DIR)/lib/libQt5Xml.so* $(1)/usr/lib/
endef

define Build/Configure
	cd $(PKG_BUILD_DIR); \
	$(CONFIGURE_VARS) \
	./configure -device linux-generic-g++ -device-option CROSS_COMPILE=mipsel-openwrt-linux-uclibc- \
		-release -opensource -confirm-license -nomake tests -nomake examples -no-eglfs -openssl-runtime \
		-no-qml-debug -no-optimized-tools -no-opengl -v -ccache -no-dbus -no-gui -no-glib -no-widgets \
		-no-egl -no-xcb-xlib -skip qtwebengine -skip qtquick3d -skip qtscxml \
		-skip qtmultimedia -skip qtlocation -skip qtserialbus -skip qt3d -prefix $(PKG_INSTALL_DIR) \
		-skip qtquickcontrols -skip qtquickcontrols2 -skip qtxmlpatterns -skip qtdoc -skip qtgamepad -skip qtpurchasing \
		-skip qtconnectivity -mips_dsp -icu\
		-device-option QMAKE_CFLAGS="$(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) -DGCC_INCOMPLETE_STD" \
		-device-option QMAKE_CXXFLAGS="$(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) -DGCC_INCOMPLETE_STD" \
		-device-option QMAKE_CPPFLAGS="$(TARGET_CXXFLAGS) $(TARGET_CPPFLAGS) -DGCC_INCOMPLETE_STD" \
		-device-option QMAKE_LFLAGS="$(TARGET_LDFLAGS)" 


endef

define Build/Compile
	$(MAKE) -j$(MAX_CPU) -C $(PKG_BUILD_DIR)/$(MAKE_PATH)
endef

define Build/Install
	$(MAKE) -j$(MAX_CPU) -C $(PKG_BUILD_DIR)/$(MAKE_PATH) install
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/
	$(CP) $(PKG_INSTALL_DIR)/* $(STAGING_DIR)/usr/
endef


$(eval $(call BuildPackage,qt-core))
$(eval $(call BuildPackage,qt-network))
$(eval $(call BuildPackage,qt-xml))
