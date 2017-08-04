cmake_minimum_required(VERSION 3.7)
project(orbit3d)

file(MAKE_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/trajectory)
file(COPY data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY physical_properties DESTINATION ${CMAKE_CURRENT_BINARY_DIR})



set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp system.cpp body.h system.h vector3.h trajectory.cpp trajectory.h planetdata.cpp planetdata.h propertiesfile.cpp propertiesfile.h)
add_executable(orbit3d ${SOURCE_FILES})
