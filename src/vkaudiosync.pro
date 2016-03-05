#  VKAudioSync
#  Copyright (C) 2011 Denis Medved <thebucefal@gmail.com>
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
QT += \
    webkit \
    network \
    xml \
    core \
    gui \
    webkitwidgets

TEMPLATE = app

TARGET = vkaudiosync

DESTDIR = bin

MOC_DIR = build

OBJECTS_DIR = build

TRANSLATIONS += \
        translations/main_en.ts \
        translations/main_ru.ts \
        translations/main_uk.ts \
        translations/main_pl.ts \
        translations/main_be.ts

CODECFORTR  = UTF-8
CODECFORSRC = UTF-8

RESOURCES += \
        icons.qrc

HEADERS += \
        audioitemdelegate.h \
        audiolistmodel.h \
        mainwindow.h \
        appsettings.h \
        synchservice.h \
        downloadthread.h \
        audioitem.h \
        vk/vkservice.h \
        profilemodel.h \
        about.h \
        appcookiejar.h \
        audiolistview.h

FORMS += \
        ui/mainwindow.ui \
        ui/about.ui

SOURCES += \
        audioitemdelegate.cpp \
        audiolistmodel.cpp \
        main.cpp \
        mainwindow.cpp \
        appsettings.cpp \
        synchservice.cpp \
        downloadthread.cpp \
        audioitem.cpp \
        vk/vkservice.cpp \
        profilemodel.cpp \
        about.cpp \
        appcookiejar.cpp \
        audiolistview.cpp


OTHER_FILES += \
        share/applications/vkaudiosync.desktop \
        ../README \
        ../AUTHORS \
        ../.gitignore

win {
    INCLUDEPATH += . bin
    LIBS += crypto ssl
   QMAKE_CXXFLAGS += -fexceptions
}

unix {
    target.path = /usr/bin

    applications.path = /usr/share/applications
    applications.files = share/applications/vkaudiosync.desktop

    icons.path = /usr/share/icons/hicolor/64x64/apps
    icons.files = share/icons/hicolor/64x64/apps/*

    data.path = /usr/share/vkaudiosync
    data.files = share/vkaudiosync/*

    INSTALLS += target applications icons data
}



