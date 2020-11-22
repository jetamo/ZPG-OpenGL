#pragma once
#include <stdio.h>
#include "Camera.h"
#include <GLFW\glfw3.h>

class Application
{

private:
    static Application instance;
public:
    Application() {};
    static bool clicked;

    static Application& getInstance()
    {
        return instance;
    }
    static void Start();
};
