#-------------------------------------------------
#  Copyright 2019 ESRI
#
#  All rights reserved under the copyright laws of the United States
#  and applicable international laws, treaties, and conventions.
#
#  You may freely redistribute and use this sample code, with or
#  without modification, provided you include the original copyright
#  notice and use restrictions.
#
#  See the Sample code usage restrictions document for further information.
#-------------------------------------------------

TEMPLATE = app

CONFIG += c++11

# additional modules are pulled in via arcgisruntime.pri
QT += opengl qml quick quickcontrols2

TARGET = GEOINTRecon

equals(QT_MAJOR_VERSION, 5) {
    lessThan(QT_MINOR_VERSION, 12) { 
        error("$$TARGET requires Qt 5.12.0")
    }
	equals(QT_MINOR_VERSION, 12) : lessThan(QT_PATCH_VERSION, 0) {
		error("$$TARGET requires Qt 5.12.0")
	}
}

ARCGIS_RUNTIME_VERSION = 100.7
include($$PWD/arcgisruntime.pri)

HEADERS += \
    AppInfo.h \
    GEOINTRecon.h \
    GeointApi.h \
    MobilePackageElement.h \
    MobilePackageStore.h \
    OperationalLayerListModel.h \
    RegularLocator.h \
    ThreatFactory.h

SOURCES += \
    GeointApi.cpp \
    MobilePackageElement.cpp \
    MobilePackageStore.cpp \
    OperationalLayerListModel.cpp \
    RegularLocator.cpp \
    ThreatFactory.cpp \
    main.cpp \
    GEOINTRecon.cpp

RESOURCES += \
    qml/qml.qrc \
    Resources/Resources.qrc

#-------------------------------------------------------------------------------

win32 {
    include (Win/Win.pri)
}

macx {
    include (Mac/Mac.pri)
}

ios {
    include (iOS/iOS.pri)
}

android {
    include (Android/Android.pri)
}
