cmake_minimum_required(VERSION 3.16)

# 定义使用 C++ 的 QMenuBar 教程项目。
# Define the QMenuBar tutorial project that uses C++.
project(QMenuBarTutorial LANGUAGES CXX)

# 使用 C++17，并禁止编译器专用扩展。
# Use C++17 and disable compiler-specific extensions.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# QMenuBar 属于 Qt Widgets 模块。
# QMenuBar belongs to the Qt Widgets module.
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 构建最小示例并链接 Qt Widgets。
# Build the minimal example and link Qt Widgets.
add_executable(QMenuBarExample main.cpp)
target_link_libraries(QMenuBarExample PRIVATE Qt6::Widgets)

# 让 MSVC 按 UTF-8 读取中英文源码。
# Make MSVC read the bilingual source as UTF-8.
if(MSVC)
    target_compile_options(QMenuBarExample PRIVATE /utf-8)
endif()

# Windows GUI 程序不显示额外的控制台窗口。
# Hide the extra console window for a Windows GUI application.
if(WIN32)
    set_target_properties(QMenuBarExample PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
