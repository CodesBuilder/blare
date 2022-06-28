#
# Top level CMakeLists.txt
#
cmake_minimum_required (VERSION 3.8)

project ("blare")

# C++ standard
set(CXX_STANDARD 11)

# Global include directories
include_directories(".")

# Subprojects
add_subdirectory ("blare")
add_subdirectory ("example")
