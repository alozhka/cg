#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <iostream>

int StartApplication(const std::function<void(GLFWwindow* window)>& onDraw)
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Hangman Game", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
        onDraw(window);
    }

    glfwTerminate();
    return 0;
}

int main()
{
    std::function onDraw = [](GLFWwindow* window)
    {
    };
    return StartApplication(onDraw);
}
