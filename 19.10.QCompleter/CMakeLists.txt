cmake_minimum_required(VERSION 3.16)

# 项目使用 C++17；这足以运行本章的最小示例。
# This project uses C++17, which is sufficient for the minimal example.
project(QCompleterTutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# QCompleter、QLineEdit 和 QApplication 都属于 Qt Widgets 模块。
# QCompleter, QLineEdit, and QApplication all belong to Qt Widgets.
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(QCompleterExample main.cpp)
target_link_libraries(QCompleterExample PRIVATE Qt6::Widgets)

# 让 MSVC 按 UTF-8 读取包含中英文注释和字符串的源文件。
# Make MSVC read the bilingual source file as UTF-8.
if(MSVC)
    target_compile_options(QCompleterExample PRIVATE /utf-8)
endif()

# 在 Windows 上构建 GUI 程序时隐藏额外的控制台窗口。
# Hide the extra console window for a Windows GUI build.
if(WIN32)
    set_target_properties(QCompleterExample PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
