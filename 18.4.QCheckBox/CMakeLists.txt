cmake_minimum_required(VERSION 3.16)

project(QCheckBoxTutorial LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(QCheckBoxExample main.cpp)
target_link_libraries(QCheckBoxExample PRIVATE Qt6::Widgets)

# 让 MSVC 按 UTF-8 读取包含中英文注释和字符串的源文件。
# Make MSVC read the bilingual comments and strings as UTF-8.
if(MSVC)
    target_compile_options(QCheckBoxExample PRIVATE /utf-8)
endif()

# 日志已经显示在窗口中，因此 Windows 下不需要额外的控制台窗口。
# The log is shown in the GUI, so no additional console window is needed on Windows.
if(WIN32)
    set_target_properties(QCheckBoxExample PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
