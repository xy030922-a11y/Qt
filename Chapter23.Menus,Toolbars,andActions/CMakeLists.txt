cmake_minimum_required(VERSION 3.16)

# 定义使用 C++ 的菜单、工具栏与操作教程项目。
# Define the C++ tutorial project for menus, toolbars, and actions.
project(MenusToolbarsActionsTutorial LANGUAGES CXX)

# 本示例使用 C++17，并禁止编译器专用扩展。
# This example uses C++17 and disables compiler-specific extensions.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 查找 Qt 6 Widgets 模块。
# Find the Qt 6 Widgets module.
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# 构建最小示例并链接 Qt Widgets。
# Build the minimal example and link Qt Widgets.
add_executable(MenusToolbarsActionsExample main.cpp)
target_link_libraries(MenusToolbarsActionsExample PRIVATE Qt6::Widgets)

# 让 MSVC 按 UTF-8 读取中英文源码。
# Make MSVC read the bilingual source as UTF-8.
if(MSVC)
    target_compile_options(MenusToolbarsActionsExample PRIVATE /utf-8)
endif()

# Windows GUI 程序不显示额外的控制台窗口。
# Do not show an extra console window for a Windows GUI application.
if(WIN32)
    set_target_properties(MenusToolbarsActionsExample PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
