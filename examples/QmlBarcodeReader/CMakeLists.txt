cmake_minimum_required(VERSION 3.16.3)

project(QmlBarcodeReader LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(USE_BINARIES ON)

set(SCODES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../src")

#Check this file for any *_DIR variable definitions and other
include("cmake/Locations.cmake")

find_package(QT NAMES Qt5 COMPONENTS Core Multimedia Concurrent Quick  REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Multimedia Concurrent Quick REQUIRED)
find_package(Threads REQUIRED)


if(Qt${QT_VERSION_MAJOR} STREQUAL Qt5)
    qt5_add_resources(RSCS Qt5qml.qrc)
else()
    qt_add_resources(RSCS Qt6qml.qrc)
endif()

if(NOT USE_BINARIES)
    add_subdirectory(${LIB_DIR} ${CMAKE_BINARY_DIR}/SCodes)
endif()

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

    add_executable(${PROJECT_NAME} main.cpp ${RSCS})

endif()

if(USE_BINARIES)

    find_package(Qt${QT_VERSION_MAJOR} COMPONENTS AndroidExtras REQUIRED)
    target_link_libraries(${PROJECT_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::AndroidExtras Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Multimedia Qt${QT_VERSION_MAJOR}::Quick)

    target_link_libraries(${PROJECT_NAME} PUBLIC EGL GLESv2)
    target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

    target_include_directories(${PROJECT_NAME} PUBLIC ${SCODES_DIR} ${SCODES_DIR}/zxing-cpp/core/src ${SCODES_DIR}/zxing-cpp/thirdparty/stb)
    target_link_libraries(${PROJECT_NAME} PUBLIC ${REQUIRED_QT_LIBS} ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/qt5/android/libZXing.a ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/qt5/android/libSCodes.a)



else()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${REQUIRED_QT_LIBS} SCodes)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
    qt_finalize_project(${PROJECT_NAME})
endif()
