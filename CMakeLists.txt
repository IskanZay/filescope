cmake_minimum_required(VERSION 3.20)
project(filescope VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus /Zc:preprocessor /utf-8 /EHsc /W4)
endif()

if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE
        "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "")
endif()

add_library(filescope_core OBJECT
    src/cli/options.cpp
)
target_include_directories(filescope_core PUBLIC src)

add_executable(filescope src/main.cpp)
target_link_libraries(filescope PRIVATE filescope_core)

enable_testing()
add_subdirectory(tests)

install(TARGETS filescope RUNTIME DESTINATION bin)