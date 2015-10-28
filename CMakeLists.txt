cmake_minimum_required(VERSION 2.8)
project(time_series_data_reader)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

include_directories(include)

set(SOURCE_FILES
        src/main.cpp
        test/test.cpp
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
        include/parse.h
        test/test.cpp)


add_executable(data_test ${SOURCE_FILES})
add_library(data ${SOURCE_FILES_NO_MAIN})

FIND_LIBRARY(GTEST_LIBRARY gtest /usr/local/lib)
TARGET_LINK_LIBRARIES(data_test ${GTEST_LIBRARY} pthread boost_thread boost_filesystem boost_system)

