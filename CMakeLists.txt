cmake_minimum_required(VERSION 3.10)
set(PROJECT_NAME cpp_app_template)
project(${PROJECT_NAME} VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Include Directories
# include_directories(${CMAKE_SOURCE_DIR}/deps//*library-name*//include)

# Set Lib Directories
# link_directories(${CMAKE_SOURCE_DIR}/deps//*library-name*//lib)

# Add source/header files for the app
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS include/*.h)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
# target_link_libraries(${PROJECT_NAME} PRIVATE)
# dependencies for libraries
# target_link_libraries(${PROJECT_NAME} PRIVATE)