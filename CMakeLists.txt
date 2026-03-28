cmake_minimum_required(VERSION 3.10)
project(videolinks-to-ascii-imGui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---- Executable ----
add_executable(main
    src/main.cpp
)

