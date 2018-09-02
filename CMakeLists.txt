cmake_minimum_required(VERSION 3.7)
project(orbit3d)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Release)

add_custom_target(copy-runtime-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/physical_properties ${CMAKE_BINARY_DIR}/physical_properties
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/trajectory
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/trajectory/vel
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/trajectory/pos)


find_package(Boost)

set(SOURCE_FILES
        main.cpp
        body.h
        system.cpp
        system.h
        vector3.h
        trajectory.cpp
        trajectory.h
        planetdata.cpp
        planetdata.h
        propertiesfile.cpp
        propertiesfile.h
        verlet.cpp
        verlet.h)
add_executable(orbit3d ${SOURCE_FILES} )
add_dependencies(orbit3d copy-runtime-files)
target_link_libraries(orbit3d
        ${Boost_LIBRARIES})