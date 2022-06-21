cmake_minimum_required(VERSION 3.12.4)

project(sysinfo)
enable_testing()

include_directories(src/)
include_directories(include/)

include_directories(${SRC_FOLDER}/ext/sqlite/)
include_directories(${SRC_FOLDER}/ext/nlohmann/)
include_directories(${SRC_FOLDER}/ext/cJSON/)
include_directories(${SRC_FOLDER}/ext/procps/)
include_directories(${SRC_FOLDER}/ext/bzip2/)
include_directories(${SRC_FOLDER}/ext/openssl/include/)
include_directories(${SRC_FOLDER}/ext/libplist/bin/include/)
include_directories(${SRC_FOLDER}/ext/libdb/build_unix/)
include_directories(${SRC_FOLDER}/shared_modules/utils)
include_directories(${SRC_FOLDER}/shared_modules/common/)

if(NOT CMAKE_CHECK_CENTOS5) # Avoid incompatible libraries in CentOS 5 and Red Hat 5
include_directories(${SRC_FOLDER}/ext/pacman/lib/libalpm/)
include_directories(${SRC_FOLDER}/ext/libarchive/libarchive/)
include_directories(${SRC_FOLDER}/ext/rpm/builddir/output/include/)
endif(NOT CMAKE_CHECK_CENTOS5)

link_directories(${SRC_FOLDER})
link_directories(${SRC_FOLDER}/ext/sqlite/)
link_directories(${SRC_FOLDER}/ext/cJSON/)
link_directories(${SRC_FOLDER}/ext/procps/)
link_directories(${SRC_FOLDER}/ext/bzip2/)
link_directories(${SRC_FOLDER}/ext/libplist/bin/lib/)
link_directories(${SRC_FOLDER}/ext/libdb/build_unix/)
link_directories(${SRC_FOLDER}/ext/openssl/)
if(NOT CMAKE_CHECK_CENTOS5) # Avoid incompatible libraries in CentOS 5 and Red Hat 5
link_directories(${SRC_FOLDER}/ext/pacman/lib/libalpm/)
link_directories(${SRC_FOLDER}/ext/libarchive/.libs/)
link_directories(${SRC_FOLDER}/ext/rpm/builddir/)
link_directories(${SRC_FOLDER}/ext/popt/build/output/src/.libs/)
link_directories(${SRC_FOLDER}/ext/zlib/)
endif(NOT CMAKE_CHECK_CENTOS5)

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  file(GLOB SYSINFO_SRC
      "src/*Win.cpp"
      "src/osinfo/sysOsInfoWin.cpp"
      "src/network/*Windows.cpp"
      "src/packages/*Windows.cpp")
  add_definitions(-DWIN32=1
                  -D_WIN32_WINNT=0x600
                  -DWIN_EXPORT)
elseif(CMAKE_CHECK_CENTOS5)
  file(GLOB SYSINFO_SRC
      "src/*Linux.cpp"
      "src/network/*Linux.cpp"
      "src/osinfo/sysOsParsers.cpp"
      "src/packages/packageLinuxParser.cpp"
      "src/packages/packageLinuxParserRpmLegacy.cpp")
  add_definitions(-DLINUX_TYPE=LinuxType::LEGACY) # Partial compilation in legacy systems
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  file(GLOB SYSINFO_SRC
      "src/*Linux.cpp"
      "src/network/*Linux.cpp"
      "src/osinfo/sysOsParsers.cpp"
      "src/packages/packageLinux*.cpp"
      "src/packages/rpm*.cpp")
  add_definitions(-DLINUX_TYPE=LinuxType::STANDARD) # Standard compilation in compatible systems
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  file(GLOB SYSINFO_SRC
      "src/*Mac.cpp"
      "src/*CommonBSD.cpp"
      "src/packages/*Mac.cpp"
      "src/network/*BSD.cpp"
      "src/osinfo/sysOsParsers.cpp")
elseif(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
  file(GLOB SYSINFO_SRC
      "src/*FreeBSD.cpp"
      "src/*CommonBSD.cpp"
      "src/network/*BSD.cpp"
      "src/osinfo/sysOsParsers.cpp")
elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
  file(GLOB SYSINFO_SRC
      "src/*OpenBSD.cpp"
      "src/*CommonBSD.cpp"
      "src/network/*BSD.cpp"
      "src/osinfo/sysOsParsers.cpp")
elseif(CMAKE_SYSTEM_NAME STREQUAL "SunOS")
  file(GLOB SYSINFO_SRC
      "src/UtilsWrapperUnix.cpp"
      "src/*Solaris.cpp"
      "src/packages/*Solaris.cpp"
      "src/network/networkSolarisHelper.cpp"
      "src/network/*Solaris.cpp"
      "src/osinfo/sysOsParsers.cpp")
else()
  file(GLOB SYSINFO_SRC
      "src/*Unix.cpp"
      "src/osinfo/sysOsParsers.cpp")
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")


add_library(sysinfo SHARED
    ${SYSINFO_SRC}
    src/sysInfo.cpp )

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  target_link_libraries(sysinfo psapi iphlpapi ws2_32)
  set_target_properties(sysinfo PROPERTIES
      PREFIX ""
      SUFFIX ".dll"
      LINK_FLAGS "-Wl,--add-stdcall-alias -static-libstdc++"
      POSITION_INDEPENDENT_CODE 0 # this is to avoid MinGW warning;
      # MinGW generates position-independent-code for DLL by default
  )
elseif(UNIX AND NOT APPLE)
  if(CMAKE_SYSTEM STREQUAL "SunOS-5.10")
    set_target_properties(sysinfo PROPERTIES
      LINK_FLAGS "-static-libstdc++")
  else()
    set_target_properties(sysinfo PROPERTIES
      LINK_FLAGS "-static-libgcc -static-libstdc++")
  endif(CMAKE_SYSTEM STREQUAL "SunOS-5.10")
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "AIX")
    string(APPEND CMAKE_SHARED_LINKER_FLAGS " -Wl,-rpath=$ORIGIN")
  else()
    string(REPLACE ";" ":" CXX_IMPLICIT_LINK_DIRECTORIES_STR "${CMAKE_CXX_IMPLICIT_LINK_DIRECTORIES}")
    string(REPLACE ";" ":" PLATFORM_REQUIRED_RUNTIME_PATH_STR "${CMAKE_PLATFORM_REQUIRED_RUNTIME_PATH}")
    target_link_libraries(sysinfo -Wl,-blibpath:${INSTALL_PREFIX}/lib:${CXX_IMPLICIT_LINK_DIRECTORIES_STR}:${PLATFORM_REQUIRED_RUNTIME_PATH_STR})
  endif(NOT CMAKE_SYSTEM_NAME STREQUAL "AIX")
elseif(APPLE)
  target_link_libraries(sysinfo cjson ${SRC_FOLDER}/external/libplist/bin/lib/libplist-2.0.a)
endif(CMAKE_SYSTEM_NAME STREQUAL "Windows")

if(APPLE)
  add_custom_command(TARGET sysinfo
    POST_BUILD COMMAND
    ${CMAKE_INSTALL_NAME_TOOL} -id "@rpath/libsysinfo.dylib"
    $<TARGET_FILE:sysinfo>)
endif(APPLE)

if(UNIT_TEST)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_link_libraries(sysinfo -fprofile-arcs)
  else()
    target_link_libraries(sysinfo gcov)
  endif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")

  add_subdirectory(tests)
else()
    #add_subdirectory(testtool)
endif(UNIT_TEST)
