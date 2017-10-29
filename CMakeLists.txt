cmake_minimum_required(VERSION 3.7)
project(orbit3d)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 11)
include_directories(${VTK_INCLUDE_DIRS})

add_custom_target(copy-runtime-files ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data ${CMAKE_BINARY_DIR}/data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/physical_properties ${CMAKE_BINARY_DIR}/physical_properties
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/trajectory)


find_package(VTK 8.0.1 REQUIRED COMPONENTS vtkViewsCore vtkGUISupportQt vtkCommonCore NO_MODULE)
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets X11Extras)
find_package(Boost)


include(${VTK_USE_FILE})

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
        scale.h
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        importdata.cpp
        importdata.h
        importdata.ui
        verlet.cpp
        verlet.h)
add_executable(orbit3d ${SOURCE_FILES} )
add_dependencies(orbit3d copy-runtime-files)
target_link_libraries(orbit3d
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::X11Extras
        ${VTK_LIBRARIES}
        ${Boost_LIBRARIES})