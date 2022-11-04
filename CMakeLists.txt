cmake_minimum_required(VERSION 3.21)

project(SCodesLibrary LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

option(USE_BINARIES "Set Binaries mode" ON)
set(USE_COMPILED_SCODE_ZXING_LIBS OFF)
set(ENABLE_EXAMPLE TRUE)

if(USE_COMPILED_SCODE_ZXING_LIBS)

else()
    add_subdirectory(src)
    if(USE_BINARIES)
        add_subdirectory(src/zxing-cpp/core)
    else()
        add_subdirectory(src/zxing-cpp)
    endif()
endif()

if(ENABLE_EXAMPLE)
   add_subdirectory(examples/QmlBarcodeReader)
endif()
