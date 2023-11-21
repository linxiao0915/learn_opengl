#include <iostream>
#include <GLFW/glfw3.h>
#include <fmt/format.h>

static void glfwErrorCallback(int error, const char* description)
{
    std::cerr << "GLFW Error " << error << ": " << description << std::endl;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

//Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfwErrorCallback);

    //初始化opengl版本
    if (!glfwInit())
    {
        return 1;
    }

    int major, minor, rev;
    glfwGetVersion(&major, &minor, &rev);

    //声明opengl版本(GL 3.0 + GLSL 130)  GLSL: opengl的着色器语言
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    std::cout << fmt::format(u8"opengl version: {}.{}.{}", major, minor, rev) << std::endl;

    //通过几何上下文创建窗体
    GLFWwindow* window = glfwCreateWindow(1280, 720, u8"你好 Dear ImGui GLFW+OpenGL3 example", nullptr, nullptr);
    if (window == nullptr)
    {
        return 1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //Enable vsync

    while (!glfwWindowShouldClose(window)) //当不关闭窗口是
    {
        glfwPollEvents(); //检查有没有触发什么事件（比如键盘输入、鼠标移动等）

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //输入
        processInput(window);

        //渲染指令
        glClearColor(0.71f, 0.21f, 0.1f, 0.1f); //清空屏幕颜色。在调用参数里设置的颜色
        glClear(GL_COLOR_BUFFER_BIT);           //清空屏幕的缓冲，可能清空的有颜色/深度/模板缓冲

        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window); //交换前后缓冲器
    }

    // //进入渲染循环
    // while (!glfwWindowShouldClose(window))
    // {
    //     //更新
    //     glfwPollEvents();
    //     ImGui_ImplOpenGL3_NewFrame();
    //     ImGui_ImplGlfw_NewFrame();
    //
    //     //开始新帧
    //     ImGui::NewFrame();
    //
    //     //显示窗体
    //     ImGui::Begin(u8"你好,世界!"); //Create a window called "Hello, world!" and append into it.
    //     ImGui::Text(u8"应用每帧耗时/帧率:  %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
    //     ImGui::End();
    //
    //     //渲染
    //     ImGui::Render();
    //     int display_w, display_h;
    //     glfwGetFramebufferSize(window, &display_w, &display_h);
    //     glViewport(0, 0, display_w, display_h);
    //     glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    //     glClear(GL_COLOR_BUFFER_BIT);
    //     ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    //
    //     //交换内存
    //     glfwSwapBuffers(window);
    // }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
