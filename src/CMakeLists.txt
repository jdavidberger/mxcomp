
file (GLOB INC  ../include/mxcomp/tuple/*.h ../include/mxcomp/*.h ../include/mxcomp/*.tcc)

add_library(mxcomp ${INC} log.cc vsostream.cc )
target_link_libraries(mxcomp )

add_subdirectory(examples)
