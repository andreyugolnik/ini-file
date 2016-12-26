PROJECT(inifile)
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

INCLUDE(FindPkgConfig)

# additional compiler flags
SET( CMAKE_CXX_FLAGS "-std=c++11 -Wall -O2" )

SET( SOURCES
    src/main.cpp
    src/Ini.cpp
    )

ADD_EXECUTABLE( inifile
    ${SOURCES} # We could've listed the source files here directly instead of using a variable to store them
    )

