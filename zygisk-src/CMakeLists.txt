cmake_minimum_required(VERSION 3.10)
project(monet_zygisk CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_library(monet_zygisk SHARED module.cpp)
target_link_libraries(monet_zygisk log)

# 与 LSPosed / ZygiskNext 自身一致：strip 掉符号和调试信息
target_compile_options(monet_zygisk PRIVATE -fvisibility=hidden -fvisibility-inlines-hidden)
target_link_options(monet_zygisk PRIVATE -s -Wl,--gc-sections )
target_compile_options(monet_zygisk PRIVATE -ffunction-sections -fdata-sections)
target_compile_options(monet_zygisk PRIVATE -fno-exceptions -fno-rtti)

set_target_properties(monet_zygisk PROPERTIES
    PREFIX ""
    OUTPUT_NAME "arm64-v8a"
    CXX_VISIBILITY_PRESET hidden)
