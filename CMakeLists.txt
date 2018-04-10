cmake_minimum_required(VERSION 3.6)
project(las_reader)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g -Wall")

set(SOURCE_FILES
        src/main.cpp
        src/DataStructures/LAS_File.cpp
        src/DataStructures/LAS_Header.cpp
        src/DataStructures/PointDataRecord.cpp
        src/DataStructures/VariableLengthRecord.cpp
        src/DataStructures/VLR_Structs.h)

include_directories(
        src/DataStructures/
)
add_executable(las_reader ${SOURCE_FILES})