add_library(Stb INTERFACE IMPORTED)
target_include_directories(Stb INTERFACE
    "${PROJECT_SOURCE_DIR}/src/zxing-cpp/thirdparty/stb")
