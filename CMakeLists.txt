cmake_minimum_required (VERSION 2.6) 
project (mxcomp) 

SET( ${PROJECT_NAME}_VERSION_MAJOR 0.0)

EXEC_PROGRAM(hg ARGS tip OUTPUT_VARIABLE hg_tip)
SET(${PROJECT_NAME}_minor_version "unknown")
STRING(REGEX REPLACE "changeset:[ \t]*([0-9]+):(.*)" "\\1" ${PROJECT_NAME}_VERSION_MINOR "${hg_tip}")
FILE(WRITE ${CMAKE_BINARY_DIR}/VERSION "v${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
FILE(READ ${CMAKE_BINARY_DIR}/VERSION ${PROJECT_NAME}_version)
message("Building ${${PROJECT_NAME}_version}")

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS} -std=c++11 -Wno-reorder -O0 -ftemplate-depth=1024")

include_directories(include)
add_subdirectory(src)

install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)
