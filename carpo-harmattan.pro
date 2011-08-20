PROJECT = carpo
TARGET = carpo
CONFIG += build_for_harmattan

include ("carpo-common.pri")

MAKEFILE = Makefile

target.path = /opt/carpo/bin
INSTALLS += target

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

icon.files = carpo.png
icon.path = /usr/share/icons/hicolor/64x64/apps
INSTALLS += icon

desktopfile.files = harmattan/carpo.desktop
desktopfile.path = /usr/share/applications
INSTALLS += desktopfile

