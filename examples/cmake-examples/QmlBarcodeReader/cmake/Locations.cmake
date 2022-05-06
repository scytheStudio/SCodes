set(COMPANY "Scythe Studio")
set(COPYRIGHT "Copyright (c) 2022 Scythe Studio. All rights reserved.")
set(IDENTIFIER "com.scythestudio.scodes.example")

# ---CONFIGURATION---
# MOST IMPORTANT PART -> change to ON/OFF if you want to use these elements in your project
option(USE_QML "Add QML support" ON)
option(USE_LIBS "Use external libraries" ON)

set(QT_NAME Qt5)

# Locations - directories in project structure
set(LIB_DIR "../../../src")
set(SRC_DIR ".")
set(RES_DIR ".")
set(QML_DIR "qml")

find_package(Qt5Core REQUIRED)

# ---PACKAGES SECTION---
# Searches the necessary modules. If you need to use an additional module (for example Qt Charts), add name to REQUIRED_PACKAGES list

list(APPEND REQUIRED_QT_PACKAGES Core Quick Gui Multimedia)

if(USE_QML)
    list(APPEND REQUIRED_QT_PACKAGES Quick)
    list(APPEND QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/${QML_DIR}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
        CACHE STRING "Qt Creator Import Path"
        FORCE)
endif()

foreach(QT_PACKAGE ${REQUIRED_QT_PACKAGES})
    list(APPEND REQUIRED_QT_LIBS ${QT_NAME}::${QT_PACKAGE})
endforeach()

find_package(QT NAMES ${QT_NAME} COMPONENTS ${REQUIRED_QT_PACKAGES} REQUIRED)
find_package(${QT_NAME} COMPONENTS ${REQUIRED_QT_PACKAGES} REQUIRED)

