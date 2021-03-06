cmake_minimum_required (VERSION 2.6) 
project (mxcomp) 

SET( ${PROJECT_NAME}_VERSION_MAJOR 0.0)

EXEC_PROGRAM(hg ${CMAKE_CURRENT_SOURCE_DIR} ARGS tip OUTPUT_VARIABLE hg_tip )
SET(${PROJECT_NAME}_minor_version "unknown")
STRING(REGEX REPLACE "changeset:[ \t]*([0-9]+):(.*)" "\\1" ${PROJECT_NAME}_VERSION_MINOR "${hg_tip}")
FILE(WRITE ${CMAKE_BINARY_DIR}/VERSION "v${${PROJECT_NAME}_VERSION_MAJOR}.${${PROJECT_NAME}_VERSION_MINOR}")
FILE(READ ${CMAKE_BINARY_DIR}/VERSION ${PROJECT_NAME}_version)
message("Building mxcomp ${${PROJECT_NAME}_version}")

set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/third)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_C_FLAGS}")

SET(IS_MAIN_PROJECT ${PROJECT_NAME}==${CMAKE_PROJECT_NAME})

include_directories(include)
add_subdirectory(src)

set(mxcompExports mxcomp)
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)

export(PACKAGE mxcomp)
install(DIRECTORY include/mxcomp DESTINATION include)

set(mxcomp_INCLUDE_DIRS "${mxcomp_INCLUDE_DIRS}")

# Create the FooBarConfig.cmake and FooBarConfigVersion files
set(REL_INCLUDE_DIR include)

set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(mxcompConfig.cmake.in
        "${PROJECT_BINARY_DIR}/mxcompConfig.cmake" @ONLY)

# ... for the install tree
set(CONF_INCLUDE_DIRS "\${MXCOMP_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(mxcompConfig.cmake.in
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/mxcompConfig.cmake" @ONLY)
# ... for both
configure_file(mxcompConfigVersion.cmake.in
        "${PROJECT_BINARY_DIR}/mxcompConfigVersion.cmake" @ONLY)

# Install the mxcompConfig.cmake and mxcompConfigVersion.cmake
install(FILES
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/mxcompConfig.cmake"
        "${PROJECT_BINARY_DIR}/mxcompConfigVersion.cmake"
        DESTINATION "lib/cmake" COMPONENT dev)
