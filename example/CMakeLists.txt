cmake_minimum_required (VERSION 3.8)

add_executable (example "lex.cpp" "lex.h" "parse.cpp" "parse.h" "main.cpp")
target_link_libraries(example blare)

add_test(NAME exampleTest COMMAND example ${SOURCE_DIR}/test.txt)
