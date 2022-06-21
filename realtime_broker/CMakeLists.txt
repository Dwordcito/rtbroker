cmake_minimum_required(VERSION 3.12.4)
project(realtime_sysinfo)

include_directories(${SRC_FOLDER}/ext/nlohmann)
include_directories(${SRC_FOLDER}/ext/sqlite3)
include_directories(${SRC_FOLDER}/sysinfo/include)
include_directories(include/)

file(GLOB RT_BROKER_SRC "src/*.cpp" "src/sqlite3/*.cpp")

add_library(rt_broker SHARED ${RT_BROKER_SRC})
