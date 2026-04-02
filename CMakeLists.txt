cmake_minimum_required(VERSION 3.10)
project(videolinks-to-ascii-imGui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Find FFmpeg manually ----
find_path(FFMPEG_INCLUDE_DIRS libavformat/avformat.h)
find_library(AVFORMAT_LIB avformat)
find_library(AVCODEC_LIB  avcodec)
find_library(AVUTIL_LIB   avutil)
find_library(SWSCALE_LIB  swscale)

if(NOT FFMPEG_INCLUDE_DIRS)
    message(FATAL_ERROR "FFmpeg headers not found. Run: sudo apt-get install -y libavformat-dev libavcodec-dev libavutil-dev libswscale-dev")
endif()
if(NOT AVFORMAT_LIB)
    message(FATAL_ERROR "libavformat not found. Run: sudo apt-get install -y libavformat-dev")
endif()
if(NOT AVCODEC_LIB)
    message(FATAL_ERROR "libavcodec not found. Run: sudo apt-get install -y libavcodec-dev")
endif()
if(NOT AVUTIL_LIB)
    message(FATAL_ERROR "libavutil not found. Run: sudo apt-get install -y libavutil-dev")
endif()
if(NOT SWSCALE_LIB)
    message(FATAL_ERROR "libswscale not found. Run: sudo apt-get install -y libswscale-dev")
endif()

message(STATUS "FFmpeg found:")
message(STATUS "  Headers:      ${FFMPEG_INCLUDE_DIRS}")
message(STATUS "  libavformat:  ${AVFORMAT_LIB}")
message(STATUS "  libavcodec:   ${AVCODEC_LIB}")
message(STATUS "  libavutil:    ${AVUTIL_LIB}")
message(STATUS "  libswscale:   ${SWSCALE_LIB}")

# ---- Executable ----
add_executable(main
    src/main.cpp
    src/VideoDecoder.cpp
)

# ---- Link FFmpeg ----
target_include_directories(main PRIVATE ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(main PRIVATE
    ${AVFORMAT_LIB}
    ${AVCODEC_LIB}
    ${AVUTIL_LIB}
    ${SWSCALE_LIB}
)
