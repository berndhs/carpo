#
# new rss reader
#

#/****************************************************************
# * This file is distributed under the following license:
# *
# * Copyright (C) 2011, Bernd Stramm
# *
# *  This program is free software; you can redistribute it and/or
# *  modify it under the terms of the GNU General Public License
# *  as published by the Free Software Foundation; either version 2
# *  of the License, or (at your option) any later version.
# *
# *  This program is distributed in the hope that it will be useful,
# *  but WITHOUT ANY WARRANTY; without even the implied warranty of
# *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# *  GNU General Public License for more details.
# *
# *  You should have received a copy of the GNU General Public License
# *  along with this program; if not, write to the Free Software
# *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
# *  Boston, MA  02110-1301, USA.
# ****************************************************************/

MYNAME = carpo

TEMPLATE = app

ICON = carpo.png


QT += core gui webkit network xml declarative
CONFIG += debug_and_release
CONFIG += mobility
MOBILITY += sensors
MOBILITY += systeminfo

INCLUDEPATH += src

QMAKE_CXXFLAGS += -std=c++0x

TRANSLATIONS = trans/carpo_de.ts \
               trans/carpo_fr.ts \

build_for_harmattan {
  MAKEFILE = Makefile
} else {
  !include ("options.pri") {
    message ("no options.pri, using defaults")
    MAKEFILE = Makefile
  } else {
    MAKEFILE = Make_$${MYNAME}
  }
}
unix {
  LIBS += -lrt
  DEFINES += USE_LINUX_RANDOM=1
} else {
  DEFINES += USE_LINUX_RANDOM=0
}

CONFIG(debug, debug|release) {
  DEFINES += DELIBERATE_DEBUG=1
  TARGET = bin/$${MYNAME}_d
  OBJECTS_DIR = tmp/debug/obj
  message ("DEBUG cxx-flags used $${QMAKE_CXXFLAGS_DEBUG}")
  message ("DEBUG c-flags used $${QMAKE_CFLAGS_DEBUG}")
} else {
  DEFINES += DELIBERATE_DEBUG=0
  TARGET = bin/$${MYNAME}
  OBJECTS_DIR = tmp/release/obj
  QMAKE_CXXFLAGS_RELEASE -= -g
  QMAKE_CFLAGS_RELEASE -= -g
  message ("RELEASE cxx-flags used $${QMAKE_CXXFLAGS_RELEASE}")
  message ("RELEASE c-flags used $${QMAKE_CFLAGS_RELEASE}")
}



UI_DIR = tmp/ui
MOC_DIR = tmp/moc
RCC_DIR = tmp/rcc
RESOURCES = $${MYNAME}.qrc 

INCLUDEPATH += ./include

FORMS = \
        ui/DebugLog.ui \
        

HEADERS = \
          include/$${MYNAME}.h \
          include/$${MYNAME}-main.h \
          include/$${MYNAME}-magic.h \
          include/cmdoptions.h \
          include/deliberate.h \
          include/delib-debug.h \
          include/version.h \
          include/headline-list.h \
          include/qml-feed-if.h \
          include/qml-control-if.h \
          include/qml-gesture-if.h \
          include/feedlist-parser.h \
          include/feedlist.h \
          include/feedlist-model.h \
          include/feedlist-writer.h \
          include/carpo-netreply.h \
          include/config-edit.h \
          include/property-store.h \
          include/topic-model.h \
          include/auto-update.h \
          include/event-special.h \
          include/orientation.h \


SOURCES = \
          src/$${MYNAME}.cpp \
          src/$${MYNAME}-main.cpp \
          src/$${MYNAME}-magic.cpp \
          src/cmdoptions.cpp \
          src/deliberate.cpp \
          src/delib-debug.cpp \
          src/version.cpp \
          src/headline-list.cpp \
          src/qml-feed-if.cpp \
          src/qml-control-if.cpp \
          src/qml-gesture-if.cpp \
          src/feedlist-parser.cpp \
          src/feedlist.cpp \
          src/feedlist-model.cpp \
          src/feedlist-writer.cpp \
          src/carpo-netreply.cpp \
          src/config-edit.cpp \
          src/property-store.cpp \
          src/topic-model.cpp \
          src/auto-update.cpp \
          src/event-special.cpp \
          src/orientation.cpp \

