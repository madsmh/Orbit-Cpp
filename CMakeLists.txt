cmake_minimum_required(VERSION 3.7)
project(orbit3d)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp system.cpp vector3.cpp body.h system.h vector3.h trajectory.cpp trajectory.h planetdata.cpp planetdata.h)
add_executable(orbit3d ${SOURCE_FILES})