if(IOS)
    set(PLATFORM "ios")
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PLATFORM "macos")
elseif(ANDROID)
    set(PLATFORM "android")
elseif(UNIX)
elseif(WIN32)
    set(PLATFORM "win32")
else()
    message("Error. Unknown operation system.")
endif()

add_library(ZXing STATIC)
set_property(TARGET ZXing PROPERTY
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/examples/QmlBarcodeReader/dependencies/ZXing/${PLATFORM}/libZXing.a")
target_include_directories(ZXing INTERFACE
    "${PROJECT_SOURCE_DIR}/src/zxing-cpp/core/src")
