/*
* OpenGL教程中文地址：https://learnopengl-cn.github.io/
* 代码来自于：https://learnopengl.com/code_viewer_gh.php?code=src/1.getting_started/1.2.hello_window_clear/hello_window_clear.cpp
*/

// 让我们试试能不能让GLFW正常工作。首先，新建一个.cpp文件，然后把下面的代码粘贴到该文件的最前面。
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// 请确认是在包含GLFW的头文件之前包含了GLAD的头文件。GLAD的头文件包含了正确的OpenGL头文件（例如GL/gl.h），
// 所以需要在其它依赖于OpenGL的头文件之前包含GLAD。

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 我们同样也希望能够在GLFW中实现一些输入控制，这可以通过使用GLFW的几个输入函数来完成。
// 我们将会使用GLFW的glfwGetKey函数，它需要一个窗口以及一个按键作为输入。这个函数将会返回这个按键是否正在被按下。
// 我们将创建一个processInput函数来让所有的输入代码保持整洁。

// 这里我们检查用户是否按下了返回键(Esc)（如果没有按下，glfwGetKey将会返回GLFW_RELEASE。
// 如果用户的确按下了返回键，我们将通过glfwSetwindowShouldClose使用把WindowShouldClose属性设置为 true的方法关闭GLFW。
// 下一次while循环的条件检测将会失败，程序将会关闭。
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // 首先，我们在main函数中调用glfwInit函数来初始化GLFW，然后我们可以使用glfwWindowHint函数来配置GLFW。
    // glfwWindowHint函数的第一个参数代表选项的名称，我们可以从很多以GLFW_开头的枚举值中选择；第二个参数接受一个整型，
    // 用来设置这个选项的值。该函数的所有的选项以及对应的值都可以在 GLFW’s window handling 这篇文档中找到。
    // 如果你现在编译你的cpp文件会得到大量的 undefined reference (未定义的引用)错误，也就是说你并未顺利地链接GLFW库。
    //
    // 由于本站的教程都是基于OpenGL 3.3版本展开讨论的，所以我们需要告诉GLFW我们要使用的OpenGL版本是3.3，
    // 这样GLFW会在创建OpenGL上下文时做出适当的调整。这也可以确保用户在没有适当的OpenGL版本支持的情况下无法运行。
    // 我们将主版本号(Major)和次版本号(Minor)都设为3。我们同样明确告诉GLFW我们使用的是核心模式(Core-profile)。
    // 明确告诉GLFW我们需要使用核心模式意味着我们只能使用OpenGL功能的一个子集（没有我们已不再需要的向后兼容特性）。
    // 如果使用的是Mac OS X系统，你还需要加下面这行代码到你的初始化代码中这些配置才能起作用（宏定义__APPLE__处）：

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 接下来我们创建一个窗口对象，这个窗口对象存放了所有和窗口相关的数据，而且会被GLFW的其他函数频繁地用到。
    // glfwCreateWindow函数需要窗口的宽和高作为它的前两个参数。第三个参数表示这个窗口的名称（标题），这里我们使用"LearnOpenGL"，
    // 当然你也可以使用你喜欢的名称。最后两个参数我们暂时忽略。这个函数将会返回一个GLFWwindow对象，我们会在其它的GLFW操作中使用到。
    // 创建完窗口我们就可以通知GLFW将我们窗口的上下文设置为当前线程的主上下文了。

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 在之前的教程中已经提到过，GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
    // 我们给GLAD传入了用来加载系统相关的OpenGL函数指针地址的函数。GLFW给我们的是glfwGetProcAddress，它根据我们编译的系统定义了正确的函数。
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // 我们接下来在渲染循环的每一个迭代中调用processInput：
        processInput(window);

        // 为了测试一切都正常工作，我们使用一个自定义的颜色清空屏幕。在每个新的渲染迭代开始的时候我们总是希望清屏，
        // 否则我们仍能看见上一次迭代的渲染结果（这可能是你想要的效果，但通常这不是）。
        // 我们可以通过调用glClear函数来清空屏幕的颜色缓冲，它接受一个缓冲位(Buffer Bit)来指定要清空的缓冲，
        // 可能的缓冲位有GL_COLOR_BUFFER_BIT，GL_DEPTH_BUFFER_BIT和GL_STENCIL_BUFFER_BIT。
        // 由于现在我们只关心颜色值，所以我们只清空颜色缓冲。
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

