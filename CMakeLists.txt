cmake_minimum_required(VERSION 2.8)
project(demo)
include(ExternalProject)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -g")
set(CMAKE_CXX_STANDARD_REQUIRED On)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${demo_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

ExternalProject_Add(
    sge
    SOURCE_DIR ${demo_SOURCE_DIR}/lib/sge
    CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${demo_BINARY_DIR}/libsge"
)

configure_file(
    ${demo_SOURCE_DIR}/config.h
    ${demo_BINARY_DIR}/config.h
)

add_executable(tilemap-demo ${demo_SOURCE_DIR}/tilemap.cpp)
add_dependencies(tilemap-demo sge)

target_include_directories(
    tilemap-demo
    PUBLIC
        ${demo_BINARY_DIR}
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
        ${SDL2_TTF_INCLUDE_DIRS}
        ${demo_BINARY_DIR}/libsge/include
)
target_link_libraries(
    tilemap-demo
    ${demo_BINARY_DIR}/libsge/lib/libsge.so
)
