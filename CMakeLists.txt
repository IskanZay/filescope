cmake_minimum_required(VERSION 3.20)
project(filescope VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Флаги MSVC
if(MSVC)
    add_compile_options(/Zc:__cplusplus /Zc:preprocessor /utf-8 /EHsc /W4)
endif()

# ── Ядро ──
add_library(filescope_core OBJECT
    src/core/FileBuffer.cpp
    src/core/file_walker.cpp
    src/core/result_store.cpp
    src/core/AnalyzerRegistry.cpp
    src/core/builtin_analyzers.cpp
    src/analyzers/magic_analyzer.cpp
    src/analyzers/entropy_analyzer.cpp
    src/cli/options.cpp
    src/cli/runner.cpp
    src/cli/output/table_printer.cpp
)

# Указываем папку src для поиска заголовков
target_include_directories(filescope_core PUBLIC src)

# ── Утилита ──
add_executable(filescope
    src/main.cpp
)
target_link_libraries(filescope PRIVATE filescope_core)

# Запуск по умолчанию
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT filescope)