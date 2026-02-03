cmake_minimum_required(VERSION 3.10)

project(LabA LANGUAGES CXX)

# 使用 C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# 定义项目根目录（labA 这一层）
set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

# 将路径传递给 C++ 代码，定义为宏 PROJECT_ROOT
add_definitions(-DPROJECT_ROOT="${ROOT_DIR}")

# 所有 cpp 文件
file(GLOB SRC_FILES
    "src/*.cpp"
)

add_executable(main ${SRC_FILES})

target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 自动创建 result 文件夹
# 这确保了在编译时，与 project 并列的 result 目录一定存在
execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_SOURCE_DIR}/../result")