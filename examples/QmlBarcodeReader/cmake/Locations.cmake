set(COMPANY "Scythe Studio")
set(COPYRIGHT "Copyright (c) 2022 Scythe Studio. All rights reserved.")
set(IDENTIFIER "com.scythestudio.scodes.example")

# ---CONFIGURATION---
option(USE_QML "Add QML support" ON)
option(USE_LIBS "Use external libraries" ON)

# Locations - directories in project structure
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)

set(SRC_DIR ".")
set(RES_DIR ".")
set(QML_DIR "qml")

# Check Qt version
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# ---PACKAGES SECTION---
list(APPEND REQUIRED_QT_PACKAGES Core Quick Gui Multimedia)

if(USE_QML)
    list(APPEND REQUIRED_QT_PACKAGES Quick)
    list(APPEND QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/${QML_DIR}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
        CACHE STRING "Qt Creator Import Path"
        FORCE)
endif()

foreach(QT_PACKAGE ${REQUIRED_QT_PACKAGES})
    list(APPEND REQUIRED_QT_LIBS Qt${QT_VERSION_MAJOR}::${QT_PACKAGE})
endforeach()

find_package(QT NAMES Qt${QT_VERSION_MAJOR} COMPONENTS ${REQUIRED_QT_PACKAGES} REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${REQUIRED_QT_PACKAGES} REQUIRED)

