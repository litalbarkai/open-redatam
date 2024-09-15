cmake_minimum_required(VERSION 3.10)
project(gui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(redatamgui main.cpp)
set_target_properties(redatamgui PROPERTIES WIN32_EXECUTABLE TRUE)

# Include directories if needed (e.g., for additional headers)
# include_directories(${CMAKE_SOURCE_DIR}/include)

# Link libraries if needed (e.g., for additional libraries)
# target_link_libraries(RedatamConverter some_library)
