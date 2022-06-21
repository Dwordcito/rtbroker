cmake_minimum_required(VERSION 3.12.4)
project(rt_testtool)

include_directories(${SRC_FOLDER}/ext/nlohmann)
include_directories(${SRC_FOLDER}/realtime_broker/include)
include_directories(include/)


link_directories(${SRC_FOLDER}/ext/libarchive/.libs/)
link_directories(${SRC_FOLDER}/ext/libdb/build_unix/)
link_directories(${SRC_FOLDER}/ext/pacman/lib/libalpm/)
link_directories(${SRC_FOLDER}/ext/popt/build/output/src/.libs/)
link_directories(${SRC_FOLDER}/ext/procps/)
link_directories(${SRC_FOLDER}/ext/rpm/builddir/)
link_directories(${SRC_FOLDER}/ext/sqlite/)
link_directories(${SRC_FOLDER}/ext/openssl/)
link_directories(${SRC_FOLDER}/ext/zlib/)

file(GLOB RT_TESTTOOL_SRC "src/*.cpp")

add_executable(rt_testtool ${RT_TESTTOOL_SRC})

target_link_libraries(rt_testtool rt_broker sysinfo sqlite3 rpm popt z ssl crypto db proc alpm archive)
