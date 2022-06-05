cmake_minimum_required(VERSION 3.14)

project(BScanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Check this file for any *_DIR variable definitions and other
include("cmake/Locations.cmake")

if(Qt${QT_VERSION_MAJOR} STREQUAL Qt5)
    qt5_add_resources(RSCS qml.qrc)
else()
    qt_add_resources(RSCS qml.qrc)
endif()

add_subdirectory(${LIB_DIR} ${CMAKE_BINARY_DIR}/SCodes)

set(PROJECT_SOURCES main.cpp ${RSCS})

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(SCodesUpd
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET SCodesUpd APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(SCodesUpd SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(SCodesUpd
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(SCodesUpd
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(SCodesUpd
  PRIVATE ${REQUIRED_QT_LIBS} SCodes)

set_target_properties(SCodesUpd PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(SCodesUpd)
    qt_finalize_executable(SCodesUpd)
endif()
