cmake_minimum_required(VERSION 3.16)

# 创建一个使用 C++17 的 Qt 示例项目。
# Create a Qt example project that uses C++17.
project(QCompleterExample LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QCompleter 和 QLineEdit 位于 Qt Widgets 模块中。
# QCompleter and QLineEdit are provided by the Qt Widgets module.
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(QCompleterExample main.cpp)
target_link_libraries(QCompleterExample PRIVATE Qt6::Widgets)

# 让 MSVC 按 UTF-8 读取双语注释和字符串。
# Make MSVC read bilingual comments and strings as UTF-8.
if(MSVC)
    target_compile_options(QCompleterExample PRIVATE /utf-8)
endif()
