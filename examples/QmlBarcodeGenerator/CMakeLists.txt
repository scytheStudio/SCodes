cmake_minimum_required(VERSION 3.16.3)

project(QmlBarcodeGenerator LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

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

if(ANDROID)

    if(Qt${QT_VERSION_MAJOR} STREQUAL Qt5)

        set(ANDROID_MIN_SDK_VERSION 21)
        set(ANDROID_ABI "armeabi-v7a")
        set(QT_ANDROID_PACKAGE_SOURCE_DIR ${PROJECT_SOURCE_DIR}/android)

        file(GLOB_RECURSE ANDROID_SOURCES
          ${QT_ANDROID_PACKAGE_SOURCE_DIR}/AndroidManifest.xml
          ${QT_ANDROID_PACKAGE_SOURCE_DIR}/build.gradle
          ${QT_ANDROID_PACKAGE_SOURCE_DIR}/gradle/wrapper/gradle-wrapper.jar
          ${QT_ANDROID_PACKAGE_SOURCE_DIR}/gradle/wrapper/gradle-wrapper.properties
          ${QT_ANDROID_PACKAGE_SOURCE_DIR}/gradlew
          ${QT_ANDROID_PACKAGE_SOURCE_DIR}/res/values/libs.xml
        )
    add_library(${PROJECT_NAME} SHARED main.cpp ${RSCS} ${ANDROID_SOURCES})

    else()

        qt_add_executable(${PROJECT_NAME} MANUAL_FINALIZATION main.cpp ${RSCS})

        set_target_properties(${PROJECT_NAME} PROPERTIES
            QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
        )

    endif()

else()

    add_executable(${PROJECT_NAME} main.cpp ${RSCS}
        ColorController.h ColorController.cpp)

endif()

target_link_libraries(${PROJECT_NAME} PRIVATE ${REQUIRED_QT_LIBS} SCodes)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
    qt_finalize_project(${PROJECT_NAME})
endif()
