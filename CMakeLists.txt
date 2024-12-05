# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10)

# Define the project name
project(MyCMakeProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(ImageProcessor src/main.cpp)
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Test executable
add_executable(ImageProcessorTests 
    test/PixelTest.cpp
    # test/ImageTest.cpp
)

target_link_libraries(ImageProcessorTests 
    ${GTEST_LIBRARIES} 
    pthread
)

set_target_properties(ImageProcessorTests PROPERTIES LINK_FLAGS "-Wl,-subsystem,console")
