#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fmt/format.h>
#include "shader.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, Shader& shader, glm::mat4& trans);

glm::mat4 view(1.0f);
Shader* shader_ptr = nullptr;

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
    glfwSetScrollCallback(window, scroll_callback);

    //初始化GLAD: 加载所有OpenGL函数指针到glad之中
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //构建并编译着色器程序
    //------------------------------------
    std::string shader_dir = std::string(SHADER_DIR);
    Shader shader(shader_dir + "/shader.vert", shader_dir + "/shader.frag");
    if (!shader.isValid())
    {
        std::cout << "Failed to load shader: " << shader.errorMessage() << std::endl;

        return -1;
    }
    shader_ptr = &shader;

    //设置顶点数据和缓冲,以及配置顶点属性
    //------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,

        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f
    };
    float textures[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f
    };
    //unsigned int indices[] = {
    //    0, 1, 3, //first triangle
    //    1, 2, 3  //second triangle
    //};

    unsigned int VAO, VBO1, VBO2, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO1);
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    //首先绑定顶点数组对象,然后绑定并设置顶点缓冲对象,再然后配置顶点属性
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(textures), textures, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //解除绑定以避免意外误操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //设置绘画的模式(点/线/填充)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //生成纹理
    //-----------
    std::string data_dir = std::string(DATA_DIR);
    //生成纹理对象1
    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //加载并生成纹理
    {
        int texture_width, texture_height, texture_channels;
        std::string texture_path = data_dir + "/textures/container.jpg";
        unsigned char* data = stbi_load(texture_path.c_str(), &texture_width, &texture_height, &texture_channels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            //释放图像数据(图像已经传递进GPU之中)
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    }

    //生成纹理对象
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    //为当前绑定的纹理对象设置环绕、过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //加载并生成纹理
    {
        int texture_width, texture_height, texture_channels;
        std::string texture_path = data_dir + "/textures/awesomeface.png";
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(texture_path.c_str(), &texture_width, &texture_height, &texture_channels, 0);
        stbi_set_flip_vertically_on_load(false);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            //释放图像数据(图像已经传递进GPU之中)
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
    }

    //为纹理定义序号
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
    shader.setFloat("mix_value", 0.2);

    //定义其变换矩阵
    glm::mat4 model(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMatrix("model", glm::value_ptr(model));

    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f)); //注意，我们将矩阵向我们要进行移动场景的反方向移动。
    shader.setMatrix("view", glm::value_ptr(view));

    glm::mat4 projection(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
    shader.setMatrix("projection", glm::value_ptr(projection));

    glm::vec3 cube_positions[] = {
        glm::vec3(-1.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };

    //进入渲染循环
    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        //处理输入
        //processInput(window, shader, trans);

        //渲染背景
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        //shader.setMatrix("model", glm::value_ptr(model));

        //更新相机矩阵
        //{
        //    float radius = 10.0f;
        //    float cam_x = sin(glfwGetTime()) * radius;
        //    float cam_z = cos(glfwGetTime()) * radius;
        //    glm::mat4 temp_view(1.0);
        //    temp_view = glm::lookAt(glm::vec3(cam_x, 0.0, cam_z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.25, 1.0, -0.3));
        //    shader.setMatrix("view", glm::value_ptr(temp_view));
        //}

        //绘画对象
        shader.use();
        glActiveTexture(GL_TEXTURE0); //在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 temp_model(1.0);
            temp_model = glm::translate(temp_model, cube_positions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0)
            {
                angle = glfwGetTime() * 25.0f;
            }
            else if (i % 3 == 1)
            {
                angle = -glfwGetTime() * 15.0f;
            }
            temp_model = glm::rotate(temp_model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMatrix("model", glm::value_ptr(temp_model));
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //交换内存并处理IO事件
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //释放资源和释放着色器
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO1);
    glDeleteBuffers(1, &VBO2);

    //结束,释放所有GLFW的资源
    glfwTerminate();
    return 0;
}

//process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
//---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, Shader& shader, glm::mat4& trans)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        trans = glm::translate(trans, glm::vec3(0.0f, 0.005f, 0.0f));
        shader.setMatrix("transform", glm::value_ptr(trans));
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        trans = glm::translate(trans, glm::vec3(0.0f, -0.005f, 0.0f));
        shader.setMatrix("transform", glm::value_ptr(trans));
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        trans = glm::translate(trans, glm::vec3(-0.005f, 0.0f, 0.0f));
        shader.setMatrix("transform", glm::value_ptr(trans));
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        trans = glm::translate(trans, glm::vec3(0.005f, 0.0f, 0.0f));
        shader.setMatrix("transform", glm::value_ptr(trans));
    }
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

//glfw: whenever the mouse scroll wheel scrolls, this callback is called
//----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
    if (yoffset > 0)
    {
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, 1.0f)); //注意，我们将矩阵向我们要进行移动场景的反方向移动。
        shader_ptr->setMatrix("view", glm::value_ptr(view));
    }
    else
    {
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f)); //注意，我们将矩阵向我们要进行移动场景的反方向移动。
        shader_ptr->setMatrix("view", glm::value_ptr(view));
    }
}