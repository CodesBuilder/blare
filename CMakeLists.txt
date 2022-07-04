#
# Top level CMakeLists.txt
#
cmake_minimum_required (VERSION 3.8)

project ("blare")
enable_testing()

# C++ standard
set(CXX_STANDARD 11)

# Global include directories
include_directories(".")

# Global Definitions
add_definitions(-D _CRT_SECURE_NO_WARNINGS -D _CRT_NONSTDC_NO_DEPRECATE)

# Subprojects
add_subdirectory ("blare")
add_subdirectory ("example")
