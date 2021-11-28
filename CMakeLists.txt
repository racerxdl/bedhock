cmake_minimum_required(VERSION 3.10)

# set the project name
project(Bedhock)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(SUBHOOK_STATIC ON)
set(BUILD_SHARED_LIBS ON)
set(CMAKE_CXX_FLAGS "-fPIC -O2")
set(CMAKE_C_FLAGS "-fPIC -O2")

add_subdirectory(subhook)

# Find source files
file(GLOB SOURCES src/hooks/*.cpp src/*.cpp)
add_library(${PROJECT_NAME} SHARED ${SOURCES})

# Include header files
include_directories(include)

target_link_libraries(${PROJECT_NAME} subhook)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE ELFIO)

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION 1.0)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION_MAJOR 1)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION_MINOR 0)
set_target_properties(${PROJECT_NAME} PROPERTIES DESCRIPTION "Bedrock Server Hooking")
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

# Install library headers
file(GLOB HEADERS include/*.h)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})