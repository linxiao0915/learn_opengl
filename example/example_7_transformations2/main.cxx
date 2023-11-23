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
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

class ItemProperty
{
public:
    glm::vec3 xyz = { 0.0f, 0.0f, 0.0f };
    float deg_angle = 0.0f; //角度制,[-360-360]
    float scale = 1.0f;
    float translate_speed = 0.005;
};

glm::mat4 getTransMatrix(const glm::vec3& xyz, float angle, float scale)
{
    glm::mat4 trans(1.0f);
    trans = glm::scale(trans, { scale, scale, scale });
    trans = glm::rotate(trans, angle, glm::vec3{ 0, 0, 1 });
    trans = glm::translate(trans, xyz);
    return trans;
}

glm::mat4 getTransMatrix(const ItemProperty& property)
{
    glm::mat4 trans(1.0f);
    trans = glm::translate(trans, property.xyz);
    trans = glm::rotate(trans, glm::radians(property.deg_angle), glm::vec3{ 0, 0, 1 });
    trans = glm::scale(trans, { property.scale, property.scale, property.scale });
    return trans;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window, ItemProperty& item_property);

int main()
{
    std::string data_dir = std::string(DATA_DIR);
    std::string shader_dir = std::string(SHADER_DIR);

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

    //初始化imgui
    //Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  //Enable Gamepad Controls
    //io.Fonts->AddFontFromFileTTF("c:/windows/fonts/msyh.ttc", 26.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());
    std::string ttf_file = data_dir + "/rcc/MSYH.TTF";
    io.Fonts->AddFontFromFileTTF(ttf_file.c_str(), 32.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

    //Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();
    ImGui::GetStyle().ScaleAllSizes(1.5);

    //Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 130";
    ImGui_ImplOpenGL3_Init(glsl_version);

    //构建并编译着色器程序
    //------------------------------------
    Shader shader(shader_dir + "/shader.vert", shader_dir + "/shader.frag");
    if (!shader.isValid())
    {
        std::cout << "Failed to load shader: " << shader.errorMessage() << std::endl;

        return -1;
    }

    //设置顶点数据和缓冲,以及配置顶点属性
    //------------------------------------
    //float vertices[] = {
    //    //positions                   // texture coords
    //    0.5f, 0.5f, 0.0f, 1.0f, 1.0f,   //top right
    //    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  //bottom right
    //    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, //bottom left
    //    -0.5f, 0.5f, 0.0f, 0.0f, 1.0f   //top left
    //};
    float vertices[] = {
        //positions                   // texture coords
        0.5f, 0.5f, 0.0f,   //top right
        0.5f, -0.5f, 0.0f,  //bottom right
        -0.5f, -0.5f, 0.0f, //bottom left
        -0.5f, 0.5f, 0.0f   //top left
    };
    float textures[] = {
        1.0f, 1.0f, //top right
        1.0f, 0.0f, //bottom right
        0.0f, 0.0f, //bottom left
        0.0f, 1.0f  //top left
    };
    unsigned int indices[] = {
        0, 1, 3, //first triangle
        1, 2, 3  //second triangle
    };

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
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //解除绑定以避免意外误操作
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    //设置绘画的模式(点/线/填充)
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    //生成纹理
    //-----------
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
    ItemProperty item_property;

    glm::mat4 trans(1.0f);
    shader.setMatrix("transform", glm::value_ptr(trans));
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    //进入渲染循环
    while (!glfwWindowShouldClose(window))
    {
        //处理输入
        processInput(window, item_property);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin(u8"控制台");
            ImGui::ColorEdit3(u8"背景颜色", (float*)&clear_color);
            if (ImGui::Button(u8"复位"))
            {
                item_property = ItemProperty();
            }
            ImGui::SliderFloat(u8"移动速度", &item_property.translate_speed, 0.001f, 0.05f);
            ImGui::SliderFloat(u8"角度", &item_property.deg_angle, -360.0f, 360.0f);
            ImGui::SliderFloat(u8"缩放", &item_property.scale, 0.05f, 2.0f);
            ImGui::Text(u8"平均耗时及帧率: %.3f ms/(%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        //渲染背景
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);

        //绘画对象
        shader.use();
        trans = getTransMatrix(item_property);
        shader.setMatrix("transform", glm::value_ptr(trans));
        glActiveTexture(GL_TEXTURE0); //在绑定纹理之前先激活纹理单元
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
void processInput(GLFWwindow* window, ItemProperty& item_property)
{
    //避免OpenGL操作和ImGui操作冲突
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_AnyWindow))
    {
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        item_property.xyz.y += item_property.translate_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        item_property.xyz.y -= item_property.translate_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        item_property.xyz.x -= item_property.translate_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        item_property.xyz.x += item_property.translate_speed;
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
