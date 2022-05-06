set(COMPANY "Scythe Studio")
set(COPYRIGHT "Copyright (c) 2022 Scythe Studio. All rights reserved.")
set(IDENTIFIER "com.scythestudio.scodes.example")

# ---CONFIGURATION---
# MOST IMPORTANT PART -> change to ON/OFF if you want to use these elements in your project
option(USE_QML "Add QML support" ON)
#option(USE_TESTS "Include tests" ON )
#option(USE_DOCUMENTATION "Add documentation based on Doxygen template" ON )
#option(USE_TRANSLATIONS "Enable using translations" ON )
option(USE_LIBS "Use external libraries" ON)

#option(WINDOWS_TARGET "Set Windows as the target platform" ON )
#option(LINUX_TARGET "Set Linux as the target platform" OFF )

set(QT_NAME Qt5)
#message("Support for Windows is ${WINDOWS_TARGET}")
#message("Support for Linux is ${LINUX_TARGET}")

# Target specific files
#set(PLATFORM_DIR "platforms")
#set(WINDOWS_PLATFORM_DIR "windows")
#set(LINUX_PLATFORM_DIR "linux")

# Locations - directories in project structure
set(LIB_DIR "../../../src")
set(SRC_DIR ".")
#set(TEST_DIR "tests")
#set(DOC_DIR "documentation")
set(RES_DIR ".")
#set(TRANS_DIR "${RES_DIR}/translations")
set(QML_DIR "qml")

find_package(Qt5Core REQUIRED)

# get absolute path to qmake, then use it to find windeployqt executable
#get_target_property(QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
#get_filename_component(QT_BIN_DIR "${QMAKE_EXECUTABLE}" DIRECTORY)

# ---PACKAGES SECTION---
# Searches the necessary modules. If you need to use an additional module (for example Qt Charts), add name to REQUIRED_PACKAGES list

list(APPEND REQUIRED_QT_PACKAGES Core Quick Gui Multimedia)
#list(APPEND NOLIB_QT_PACKAGES LinguistTools)
if(USE_QML)
    list(APPEND REQUIRED_QT_PACKAGES Quick)
    list(APPEND QML_IMPORT_PATH "${CMAKE_SOURCE_DIR}/${QML_DIR}")
    set(QML_IMPORT_PATH ${QML_IMPORT_PATH}
        CACHE STRING "Qt Creator Import Path"
        FORCE)
endif()
#if(USE_TESTS)
#    list(APPEND REQUIRED_QT_PACKAGES Test)
#endif()
foreach(QT_PACKAGE ${REQUIRED_QT_PACKAGES})
    list(APPEND REQUIRED_QT_LIBS ${QT_NAME}::${QT_PACKAGE})
endforeach()

find_package(QT NAMES ${QT_NAME} COMPONENTS ${REQUIRED_QT_PACKAGES} #[[${NOLIB_QT_PACKAGES}]] REQUIRED)
find_package(${QT_NAME} COMPONENTS ${REQUIRED_QT_PACKAGES} #[[${NOLIB_QT_PACKAGES}]] REQUIRED)

