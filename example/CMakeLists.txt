cmake_minimum_required (VERSION 3.8)

file(GLOB_RECURSE SRC *.c *.h  *.cpp *.cc *.cxx *.hpp *.hh *.hxx)
add_executable (example ${SRC})
target_link_libraries(example blare)

add_test(NAME exampleTest COMMAND example ${SOURCE_DIR}/test.txt)
