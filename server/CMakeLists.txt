cmake_minimum_required(VERSION 2.8.12)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Use C++ 11 standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Project settings
project("VN Streamer")
add_definitions(-DVERSION="0.1.0a")
add_definitions(-DPROJECT_GITHUB_URL="https://github.com/Bionus/vn-streamer")

# Folder structure
add_subdirectory(lib)
add_subdirectory(gui)
