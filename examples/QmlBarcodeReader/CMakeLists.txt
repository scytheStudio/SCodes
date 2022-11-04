include(Locations)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 COMPONENTS Core Multimedia Qml Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Multimedia Qml Quick REQUIRED)

if(USE_COMPILED_SCODE_ZXING_LIBS)
    find_package(SCodes REQUIRED)
endif()

if(Qt${QT_VERSION_MAJOR} STREQUAL Qt5)
    qt5_add_resources(RSCS Qt5qml.qrc)
else()
    qt_add_resources(RSCS Qt6qml.qrc)
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

        add_executable(QmlBarcodeReader
            ${ANDROID_SOURCES}
            ${RSCS}
            main.cpp
        )

    else()
        qt_add_executable(QmlBarcodeReader MANUAL_FINALIZATION main.cpp ${RSCS})

        set_target_properties(QmlBarcodeReader PROPERTIES
            QT_ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android"
        )
    endif()
else()
    add_executable(QmlBarcodeReader
        ${RSCS}
        main.cpp
     )
endif()

target_link_libraries(QmlBarcodeReader PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
    SCodes
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(QmlBarcodeReader)
    qt_finalize_executable(QmlBarcodeReader)
    qt_finalize_project(QmlBarcodeReader)
endif()
