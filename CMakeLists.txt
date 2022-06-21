cmake_minimum_required(VERSION 3.12.4)
project(realtime_sysinfo)

get_filename_component(SRC_FOLDER     ${CMAKE_SOURCE_DIR}/ ABSOLUTE)
include_directories(${SRC_FOLDER}/utils)
link_directories(${SRC_FOLDER}/ext/sqlite3/.libs)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Woverloaded-virtual -Wunused -Wcast-align -Wformat=2 -std=c++17 -pthread -g")

add_subdirectory("realtime_broker")
add_subdirectory("sysinfo")
add_subdirectory("test_tool")
