#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include "shader_source.hpp"

/*
 * 术语:
 *      顶点数组对象：Vertex Array Object，VAO
 *      顶点缓冲对象：Vertex Buffer Object，VBO
 *      元素缓冲对象：Element Buffer Object，EBO 或
 *      索引缓冲对象: Index Buffer Object，IBO
 *
 *      图形渲染管线: Graphics Pipeline, 3D转换到2D的管理器
 *      着色器: Shader, 实现图像渲染,用来替代固定渲染管线的可编辑程序
 *
 */

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

int main()
{
    //初始化并配置glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //创建glfw窗口
    constexpr int width = 1280;
    constexpr int height = 760;
    GLFWwindow* window = glfwCreateWindow(width, height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //初始化GLAD: 加载所有OpenGL函数指针到glad之中
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //构建并编译着色器程序
    //------------------------------------
    //vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER); //创建顶点着色器
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL); //导入顶点着色器程序
    glCompileShader(vertex_shader);                                //编译顶点着色器
    //check for shader compile errors
    int success;
    char info_log[512] = { 0 };
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                  << info_log << std::endl;
    }
    //fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER); //创建片段着色器
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL); //导入片段着色器程序
    glCompileShader(fragment_shader);                                  //编译片段着色器
    //check for shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                  << info_log << std::endl;
    }
    //链接着色器程序
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << info_log << std::endl;
    }
    //链接完成后,即可释放顶点着色器和编译着色器
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    //设置顶点数据和缓冲,以及配置顶点属性
    //------------------------------------
    //定义三角形顶点
    float vertices1[] = {
        0.25f, -0.25f, 0.0f,
        0.75f, -0.25f, 0.0f,
        0.5f, -0.75f, 0.0f
    };

    unsigned int VBO1, VAO1;
    glGenVertexArrays(1, &VAO1);
    glGenBuffers(1, &VBO1);
    //首先绑定顶点数组对象,然后绑定并设置顶点缓冲对象,再然后配置顶点属性
    glBindVertexArray(VAO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //告诉OpenGL如何解析顶点数据
    glEnableVertexAttribArray(0);

    float vertices2[] = {
        -0.25f, 0.25f, 0.0f,
        -0.75f, 0.25f, 0.0f,
        -0.5f, 0.75f, 0.0f
    };
    unsigned int VBO2, VAO2;
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO2);
    //首先绑定顶点数组对象,然后绑定并设置顶点缓冲对象,再然后配置顶点属性
    glBindVertexArray(VAO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //告诉OpenGL如何解析顶点数据
    glEnableVertexAttribArray(0);

    //构建矩形顶点
    float vertices3[] = {

        0.25f, 0.75f, 0.0f, //左上角
        0.75f, 0.75f, 0.0f, //右上角
        0.75f, 0.25f, 0.0f, //右下角
        0.25f, 0.25f, 0.0f, //左下角
    };
    unsigned int VBO3, VAO3;
    glGenVertexArrays(1, &VAO3);
    glGenBuffers(1, &VBO3);
    //首先绑定顶点数组对象,然后绑定并设置顶点缓冲对象,再然后配置顶点属性
    glBindVertexArray(VAO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); //告诉OpenGL如何解析顶点数据
    glEnableVertexAttribArray(0);

    //构建矩形顶点的EBO
    unsigned int indices3[] = {
        0, 1, 3, //第一个三角形
        1, 2, 3  //第二个三角形
    };
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices3), indices3, GL_STATIC_DRAW);

    //解除绑定以避免意外误操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //设置绘画的模式(点/线/填充)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //进入渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //处理输入
        processInput(window);

        //渲染背景
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //绘画对象
        glUseProgram(shader_program);
        glBindVertexArray(VAO1); //通过VAO调用对应的数据
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO2); //通过VAO调用对应的数据
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(VAO3); //通过VAO调用对应的数据
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //交换内存并处理IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //释放资源和释放着色器
    glDeleteVertexArrays(1, &VAO1);
    glDeleteBuffers(1, &VBO1);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteProgram(shader_program);

    //结束,释放所有GLFW的资源
    glfwTerminate();
    return 0;
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//glfw: whenever the window size changed (by OS or user resize) this callback function executes
//---------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    //make sure the viewport matches the new window dimensions; note that width and
    //height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
