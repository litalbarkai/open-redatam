cmake_minimum_required(VERSION 3.14)

# Project name
project(redatamgui)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt5
set(CMAKE_PREFIX_PATH "C:/Qt/5.15.2/msvc2019_64/lib/cmake")  # Adjust this path to your Qt installation
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Include directories
include_directories(
    ${Qt5Widgets_INCLUDE_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Source files
set(GUI_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
)

# Debug output to check if files are found
message(STATUS "GUI_FILES: ${GUI_FILES}")

# Check if any source files were found
if(NOT GUI_FILES)
    message(FATAL_ERROR "No GUI source files found. Please check the paths.")
endif()

# Qt5 wrap MOC
qt5_wrap_cpp(MOC_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h)

# Add executable
add_executable(${PROJECT_NAME} ${GUI_FILES} ${MOC_SOURCES})

# Link Qt5 libraries
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
