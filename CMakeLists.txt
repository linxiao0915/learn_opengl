cmake_minimum_required(VERSION 3.26)

project(learn_opengl)

#################################################################################
# 设置C++编译器选项
#################################################################################

set(CMAKE_CXX_STANDARD 11)              # 使用C++11标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)     # 使用C++
set(CMAKE_INCLUDE_CURRENT_DIR ON)       # 包含当前目录
set(CMAKE_BUILD_TYPE "Release")         # release模式去编译

if (WIN32)
    # 开启此选项,可以在release模式下进行debug调试
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF")
else ()
    add_compile_options(-fPIC)
endif ()

#################################################################################
# 第三方库
#################################################################################

if (WIN32)
    include(cmake/find_dependencies_window.cmake)
else ()
    include(cmake/find_dependencies_linux.cmake)
endif ()

#################################################################################
# 系统参数
#################################################################################


#################################################################################
# 添加工程路径
#################################################################################

add_subdirectory(example)