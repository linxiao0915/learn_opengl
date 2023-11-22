//
// Created by cxc on 2023/11/22.
//

#include "shader.hpp"

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const std::string& vertex_path, const std::string& fragment_path)
{
    int temp_success = false;

    //加载顶点着色器程序
    //---------------------------
    //打开文件
    std::ifstream fs_vs(vertex_path, std::ios_base::in);
    if (!fs_vs.is_open())
    {
        error_message_ = u8"打开顶点着色器失败,文件无效";
        return;
    }
    //获取其文本内容
    std::stringstream ss_vs;
    ss_vs << fs_vs.rdbuf();
    fs_vs.close();
    code_vs_ = ss_vs.str();
    if (code_vs_.empty())
    {
        error_message_ = u8"顶点着色器内容为空";
        return;
    }
    //构建并编译着色器程序
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    const char* code_vs_c = code_vs_.c_str();
    glShaderSource(vertex_shader, 1, &code_vs_c, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &temp_success);
    if (!temp_success)
    {
        char info_log[512] = { 0 };
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        error_message_ = u8"编译顶点着色器失败,错误原因: " + std::string(info_log);
        return;
    }

    //加载片段着色器程序
    //---------------------------
    //打开文件
    std::ifstream fs_fs(fragment_path, std::ios_base::in);
    if (!fs_fs.is_open())
    {
        error_message_ = u8"打开片段着色器失败,文件无效";
        return;
    }
    //获取其文本内容
    std::stringstream ss_fs;
    ss_fs << fs_fs.rdbuf();
    fs_fs.close();
    code_fs_ = ss_fs.str();
    if (code_fs_.empty())
    {
        error_message_ = u8"片段着色器内容为空";
        return;
    }
    //构建并编译着色器程序
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* code_fs_c = code_fs_.c_str();
    glShaderSource(fragment_shader, 1, &code_fs_c, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &temp_success);
    if (!temp_success)
    {
        char info_log[512] = { 0 };
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        error_message_ = u8"编译片段着色器失败,错误原因: " + std::string(info_log);
        return;
    }

    //链接着色器程序
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &temp_success);
    if (!temp_success)
    {
        char info_log[512] = { 0 };
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        error_message_ = u8"链接片段着色器失败,错误原因: " + std::string(info_log);
        return;
    }

    //链接完成后,即可释放顶点着色器和编译着色器
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    is_valid_ = true;
    shader_program_ = shader_program;
}

Shader::~Shader()
{
    if (is_valid_)
    {
        glDeleteProgram(shader_program_);
    }
}

void Shader::use()
{
    glUseProgram(shader_program_);
}

std::string Shader::errorMessage() const
{
    return error_message_;
}

bool Shader::isValid() const
{
    return is_valid_;
}
unsigned int Shader::id() const
{
    return shader_program_;
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), (int)value);
}
void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(shader_program_, name.c_str()), value);
}
void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(shader_program_, name.c_str()), value);
}