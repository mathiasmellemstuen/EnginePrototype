#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


class Window {
    public:
        GLFWwindow* glfwWindow; 
        Window(); 
        ~Window();
};