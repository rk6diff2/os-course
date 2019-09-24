cmake_minimum_required(VERSION 3.14)
project(lab2 C)

set(CMAKE_C_STANDARD 99)
message(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB ALL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
        ${CMAKE_CURRENT_SOURCE_DIR}/main.c)

file(GLOB ALL_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)


add_executable(${PROJECT_NAME} ${ALL_SOURCES} ${ALL_HEADERS})
