cmake_minimum_required(VERSION 3.7)
project(orbit3d)

set(CMAKE_CXX_STANDARD 14)

add_custom_target(copy-runtime-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/physical_properties ${CMAKE_BINARY_DIR}/physical_properties
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/trajectory
        )


set(SOURCE_FILES main.cpp system.cpp body.h system.h vector3.h trajectory.cpp trajectory.h planetdata.cpp planetdata.h propertiesfile.cpp propertiesfile.h)
add_executable(orbit3d ${SOURCE_FILES} )
add_dependencies(orbit3d copy-runtime-files)
