cmake_minimum_required(VERSION 3.3)
project(time_series_data_reader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp time_series_file_reader.cpp time_series_file_reader.h time_series.h file_comparer.cpp file_comparer.h time_series_file_writer.cpp time_series_file_writer.h foreach_macro.h)
add_executable(time_series_data_reader ${SOURCE_FILES})