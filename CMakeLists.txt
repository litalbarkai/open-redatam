cmake_minimum_required(VERSION 3.10)
project(RedatamConverter)

set(CMAKE_CXX_STANDARD 17)

# Xerces path
set(XERCES_PATH "D:/a/redatam-converter/redatam-converter/xerceswin")

# Include directories
include_directories(include include/entities include/readers include/exporters ${XERCES_PATH}/include)

# Source files
file(GLOB_RECURSE SRCS "src/*.cpp")

# Find Qt5
set(Qt5_DIR "C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5")
find_package(Qt5 COMPONENTS Widgets REQUIRED)

# Executables
add_executable(redatam ${SRCS})

# Link libraries
target_link_libraries(redatam ${XERCES_PATH}/lib/xerces-c_3.lib)

if(BUILD_GUI)
    file(GLOB_RECURSE GUI_SRCS "gui/*.cpp")
    add_executable(redatamgui ${SRCS} ${GUI_SRCS})
    target_link_libraries(redatamgui ${XERCES_PATH}/lib/xerces-c_3.lib Qt5::Widgets)
endif()
