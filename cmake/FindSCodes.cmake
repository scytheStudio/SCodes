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

find_package(ZXing REQUIRED)

add_library(SCodes STATIC )
set_property(TARGET SCodes PROPERTY
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/examples/QmlBarcodeReader/dependencies/SCodes/${PLATFORM}/libSCodes.a")
target_link_libraries(SCodes INTERFACE
    ZXing)
target_include_directories(SCodes INTERFACE
    "${PROJECT_SOURCE_DIR}/examples/QmlBarcodeReader/dependencies/SCodes/include")
