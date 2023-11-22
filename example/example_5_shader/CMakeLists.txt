cmake_minimum_required(VERSION 3.8)

project("example_5_shader")

#################################################################################
# 设置C++编译器选项
#################################################################################

#################################################################################
# 第三方库
#################################################################################

# 添加glfw-3.3.8
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${CUSTOM_LIB_DIR}/glfw-3.3.8")
if (EXISTS ${CUSTOM_LIB_DIR}/glfw-3.3.8)
    include_directories(${CUSTOM_LIB_DIR}/glfw-3.3.8/include)
    link_directories(${CUSTOM_LIB_DIR}/glfw-3.3.8/lib)
    message(STATUS "The glfw lib is found!")
else ()
    message(STATUS "The glfw lib is no found!")
endif ()

# 添加glad
include_directories(${CMAKE_SOURCE_DIR}/3rdparty/glad-3.3/include)
set(glad_src ${CMAKE_SOURCE_DIR}/3rdparty/glad-3.3/src/glad.c)

#################################################################################
# 系统参数
#################################################################################

add_definitions(-DSHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shader")

#################################################################################
# 添加工程路径
#################################################################################

set(PROJECT_SOURCES
        ${glad_src}
        main.cxx
        shader.cxx
        shader.hpp
)

# 将源代码添加到此项目的可执行文件。
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_link_libraries(
        ${PROJECT_NAME}
        fmt
        ${Boost_LIBRARIES}
        glfw3
)

####################################################################################################
# 安装
####################################################################################################
