cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SRC *.c *.h  *.cpp *.cc *.cxx *.hpp *.hh *.hxx)
add_library (blare ${SRC})

install(TARGETS blare)
