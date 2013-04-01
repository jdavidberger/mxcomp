IF( ${PROJECT_NAME} STREQUAL ${CMAKE_PROJECT_NAME})
    MESSAGE("Making examples")
    add_executable(tuples tuples.cc)
    add_executable(tuples2 tuples2.cc)
    target_link_libraries(tuples2 )

    add_executable(timing timing.cc)

    include_directories(/usr/include/python2.7)
    add_library(People SHARED python.cc)
    target_link_libraries(People stencet boost_python python2.7)
ENDIF()
