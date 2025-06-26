#ifndef WINDOW_H
#define WINDOW_H
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window
{
    private:
    GLFWwindow* windowRef{};
    int width{800};
    int height{600};
    int bufferWidth{800};
    int bufferHeight{600};
    bool cursorLocked{false};
    float lastX = width / 2.0f;
    float lastY = height / 2.0f;

    void CreateCallbacks();
    void ToggleCursor(GLFWwindow* window);
    static void Resize(GLFWwindow* window, int width, int height);
    static void SetMousePos(GLFWwindow* window, double xPos, double yPos);

    public:
    Window();
    Window(int width, int height);
    ~Window();

    void Initialize();
    void ProcessInput();
    bool GetShouldClose();
    void SwapBuffers();
};

#endif