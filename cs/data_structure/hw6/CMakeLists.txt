cmake_minimum_required(VERSION 3.10)
project(DataStructure_HW6 CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 告诉编译器在哪里找头文件 (当前目录)
include_directories(.)

# 添加用于测试的主可执行文件
# 所有的源文件都应该包含在这里
add_executable(
    hw6_test 
    DFS_G_test.cpp   # 主测试驱动文件
    DFS_G.cpp        # DFS 实现
    PathExist.cpp    # PathExists 实现
)