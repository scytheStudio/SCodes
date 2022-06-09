include("../../src/SCodes.pri")

QT += quick
CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp

equals(QT_MAJOR_VERSION, 6) {
    RESOURCES += Qt6qml.qrc
}

equals(QT_MAJOR_VERSION, 5) {
    RESOURCES += Qt5qml.qrc
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {
    DISTFILES += \
        android/AndroidManifest.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew \
        android/gradlew.bat \
        android/res/values/libs.xml

    ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

    ANDROID_ABIS = armeabi-v7a
}

