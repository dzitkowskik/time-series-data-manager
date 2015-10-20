cmake_minimum_required(VERSION 2.8)
project(time_series_data_reader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(include)

set(SOURCE_FILES
        src/main.cpp
        include/foreach_macro.h
        src/file.cpp
        include/file.h
        include/ts_file_definition.h
        src/time_series.cpp
        include/time_series.h
        include/parse.h)

set(SOURCE_FILES_NO_MAIN
        include/foreach_macro.h
        src/file.cpp
        include/file.h
        include/ts_file_definition.h
        src/time_series.cpp
        include/time_series.h
        include/parse.h)

add_executable(time_series_data_reader ${SOURCE_FILES})
add_library(ddj_tsfm STATIC ${SOURCE_FILES_NO_MAIN})
