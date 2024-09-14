cmake_minimum_required(VERSION 3.10)
project(RedatamConverter)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(include include/entities include/readers include/exporters ${CMAKE_SOURCE_DIR}/xerceswin/include)

# Source files
file(GLOB_RECURSE SRCS "src/*.cpp")

# Find Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Executables
add_executable(redatam ${SRCS})

# Link libraries
target_link_libraries(redatam ${CMAKE_SOURCE_DIR}/xerceswin/lib/xerces-c_3.lib)

if(BUILD_GUI)
    file(GLOB_RECURSE GUI_SRCS "gui/*.cpp")
    add_executable(redatamgui ${SRCS} ${GUI_SRCS})
    target_link_libraries(redatamgui ${CMAKE_SOURCE_DIR}/xerceswin/lib/xerces-c_3.lib Qt5::Widgets)
endif()
