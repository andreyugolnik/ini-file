cmake_minimum_required(VERSION 3.22)
project(inifile)

include(FindPkgConfig)

# additional compiler flags
set(CMAKE_CXX_FLAGS "-std=c++11 -Wall -O2")

set(SOURCES
    src/main.cpp
    src/Ini.cpp
)

add_executable(inifile
    # We could've listed the source files here directly
    # instead of using a variable to store them
    ${SOURCES}
)

