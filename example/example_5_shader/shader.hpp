//
// Created by cxc on 2023/11/22.
//

#pragma once

#include <string>

class Shader
{
public:
    Shader(const std::string& vertex_path, const std::string& fragment_path);
    ~Shader();

    //使用/激活程序
    void use();

    //状态
    std::string errorMessage() const;
    bool isValid() const;

    //uniform工具函数
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    bool is_valid_ = false;
    std::string error_message_;
    std::string code_vs_;
    std::string code_fs_;
    unsigned int shader_program_ = 0;
};
