
file (GLOB INC  ../include/mxcomp/tuple/*.h ../include/mxcomp/*.h ../include/mxcomp/*.tcc)

if (MSVC)
  add_library(mxcomp ${INC} utf.cc log.cc vsostream.cc )
else()
  add_library(mxcomp ${INC} utf.cc log.cc )
  target_link_libraries(mxcomp pthread)
endif(MSVC)
add_subdirectory(examples)


include_directories( . )
add_executable(test_utf test_utf.cc)
target_link_libraries(test_utf mxcomp)
