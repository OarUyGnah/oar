# CMakeList.txt: oar 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project ("oar")

# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)
# 添加子目录src
add_subdirectory(src)
# 将源代码添加到此项目的可执行文件。
add_executable (oar ${DIR_SRCS} )

#要将add_executable放在target_link_libraries前
# 添加链接库
target_link_libraries(oar Timer)
# 将源代码添加到此项目的可执行文件。
#add_executable (oar "oar.cpp" "oar.h" "src/oartimer.cpp" "src/oartimer.h")


# TODO: 如有需要，请添加测试并安装目标。
