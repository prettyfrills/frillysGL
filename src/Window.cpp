#include "Window.h"
#include <iostream>
#include <GLFW/glfw3.h>

Window::Window()
: width(800), height(600)
{}

Window::Window(int inWidth, int inHeight)
: width(inWidth), height(inHeight)
{}

Window::~Window()
{
    glfwDestroyWindow(windowRef);
    glfwTerminate();
}

void Window::Initialize()
{
    // Initialize GLFW.
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    windowRef = glfwCreateWindow(width, height, "FrillysGL", NULL, NULL);
    if(!windowRef)
    {
        std::cerr << "Failed to create glfw window. Aborting." << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(windowRef);

    // Initialize GLAD.
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize OpenGL functions with GLAD. Aborting." << std::endl;
        return;
    }

    glViewport(0, 0, width, height);
}

void Window::ProcessInput()
{
}

void Window::ToggleCursor(GLFWwindow* window)
{
    if(!cursorLocked)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        cursorLocked = true;
        return;
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    cursorLocked = false;
}

void Window::CreateCallbacks()
{
    glfwSetFramebufferSizeCallback(windowRef, Resize);
    glfwSetCursorPosCallback(windowRef, SetMousePos);
}

void Window::Resize(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Window::SetMousePos(GLFWwindow* window, double xPos, double yPos)
{

}

bool Window::GetShouldClose()
{
    return glfwWindowShouldClose(windowRef);
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(windowRef);
}