#include "window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define WIDTH 800
#define HEIGHT 600
#define TITLE "Hello Vulkan"

Window::Window() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    glfwWindow = glfwCreateWindow(WIDTH, HEIGHT, TITLE, nullptr, nullptr);
};

Window::~Window() {
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
};