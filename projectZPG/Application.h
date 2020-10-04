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
    
    static void window_size_callback(GLFWwindow* window, int width, int height) {
        printf("resize %d, %d \n", width, height);
        glViewport(0, 0, width, height);
    }

    static void window_iconify_callback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

    static void window_focus_callback(GLFWwindow* window, int focused) { printf("window_focus_callback \n"); }

    static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);
        printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
    }

    static void error_callback(int error, const char* description) { fputs(description, stderr); }

    static void button_callback(GLFWwindow* window, int button, int action, int mode) {
        if (action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);
    }
};

Application Application::instance;