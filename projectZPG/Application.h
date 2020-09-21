#pragma once
#include <GLFW\glfw3.h>
#include <stdio.h>
class Application
{

private:
    static Application instance;
    
public:
    Application() {};

    static Application& getInstance()
    {
        return instance;
    }
    void cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY) {
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        printf("cursor_pos_callback %d, %d \n", (int)x, (int)y);
    }
    
};

Application Application::instance;