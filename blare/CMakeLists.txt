# CMakeList.txt: blare 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
file(GLOB_RECURSE SRC *.c *.h  *.cpp *.cc *.cxx *.hpp *.hh *.hxx)
add_library (blare ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET blare PROPERTY CXX_STANDARD 11)
endif()

# TODO: 如有需要，请添加测试并安装目标。
