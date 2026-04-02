cmake_minimum_required(VERSION 3.10)
project(videolinks-to-ascii-imGui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Find FFmpeg via pkg-config ----
find_package(PkgConfig REQUIRED)

pkg_check_modules(AVFORMAT libavformat)
pkg_check_modules(AVCODEC libavcodec)
pkg_check_modules(AVUTIL libavutil)
pkg_check_modules(SWSCALE libswscale)

if(NOT AVFORMAT_FOUND)
    message(FATAL_ERROR "FFmpeg: libavformat not found. Run: sudo apt-get install -y libavformat-dev")
endif()

if(NOT AVCODEC_FOUND)
    message(FATAL_ERROR "FFmpeg: libavcodec not found. Run: sudo apt-get install -y libavcodec-dev")
endif()

if(NOT AVUTIL_FOUND)
    message(FATAL_ERROR "FFmpeg: libavutil not found. Run: sudo apt-get install -y libavutil-dev")
endif()

if(NOT SWSCALE_FOUND)
    message(FATAL_ERROR "FFmpeg: libswscale not found. Run: sudo apt-get install -y libswscale-dev")
endif()

message(STATUS "FFmpeg found:")
message(STATUS "  libavformat version: ${AVFORMAT_VERSION}")
message(STATUS "  libavcodec  version: ${AVCODEC_VERSION}")
message(STATUS "  libavutil   version: ${AVUTIL_VERSION}")
message(STATUS "  libswscale  version: ${SWSCALE_VERSION}")

# ---- Executable ----
add_executable(main
    src/main.cpp
    src/VideoDecoder.cpp
)

# ---- Link FFmpeg ----
target_include_directories(main PRIVATE
    ${AVFORMAT_INCLUDE_DIRS}
    ${AVCODEC_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
    ${SWSCALE_INCLUDE_DIRS}
)

target_link_libraries(main PRIVATE
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
)

target_compile_options(main PRIVATE
    ${AVFORMAT_CFLAGS_OTHER}
    ${AVCODEC_CFLAGS_OTHER}
    ${AVUTIL_CFLAGS_OTHER}
    ${SWSCALE_CFLAGS_OTHER}
)
