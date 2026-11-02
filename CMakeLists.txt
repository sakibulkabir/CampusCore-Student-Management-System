cmake_minimum_required(VERSION 3.16)
project(CampusCore LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_executable(CampusCore
    src/main.cpp
    src/StudentManager.cpp
    src/ConsoleUI.cpp
)
target_include_directories(CampusCore PRIVATE include)
if(MSVC)
    target_compile_options(CampusCore PRIVATE /W4)
else()
    target_compile_options(CampusCore PRIVATE -Wall -Wextra -pedantic)
endif()
