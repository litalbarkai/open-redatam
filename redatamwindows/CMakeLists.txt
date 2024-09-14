cmake_minimum_required(VERSION 3.10)
project(redatam)

set(CMAKE_CXX_STANDARD 17)

# Paths
set(XERCES_PATH "Z:/xerceswin")
set(SRC_PATH "Z:/src")
set(INCLUDE_PATH "Z:/include")

# Debug: Print the paths being used
message(STATUS "Xerces path: ${XERCES_PATH}")
message(STATUS "Source path: ${SRC_PATH}")
message(STATUS "Include path: ${INCLUDE_PATH}")

# Verify that the paths exist
foreach(path ${SRC_PATH} ${INCLUDE_PATH} ${GUI_PATH})
    if(NOT EXISTS ${path})
        message(FATAL_ERROR "Path does not exist: ${path}")
    endif()
endforeach()

# Include directories
include_directories(${XERCES_PATH}/include)
include_directories(${INCLUDE_PATH})
include_directories(${INCLUDE_PATH}/entities)
include_directories(${INCLUDE_PATH}/readers)
include_directories(${INCLUDE_PATH}/exporters)

# Debug: Print the include directories
message(STATUS "Include directories: ${XERCES_PATH}/include, ${INCLUDE_PATH}, ${INCLUDE_PATH}/entities, ${INCLUDE_PATH}/readers, ${INCLUDE_PATH}/exporters, ${GUI_PATH}")

# Source files
file(GLOB_RECURSE REDATAM_SRCS "${SRC_PATH}/*.cpp")

# Debug: Print the source files found
message(STATUS "Source files for redatam: ${REDATAM_SRCS}")

# Check if any source files were found
if(NOT REDATAM_SRCS)
    message(FATAL_ERROR "No source files found for redatam")
endif()

# Executable for redatam
add_executable(redatam ${REDATAM_SRCS} ${SRC_PATH}/main.cpp)

# Link libraries
target_link_libraries(redatam ${XERCES_PATH}/lib/xerces-c_3.lib)
