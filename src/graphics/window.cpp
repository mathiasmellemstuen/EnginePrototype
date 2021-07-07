#include "window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../utility/properties.h"
#include "../utility/log.h"

Window::Window() {

    log(INFO, "Intializing GLFW"); 

    if(glfwInit() == GLFW_FALSE) {
        log(ERROR, "Error while initializing GLFW!"); 
    }
    log(SUCCESS, "GLFW initiated!");
    log(INFO, "Creating GLFW window context"); 

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindow = glfwCreateWindow(properties.screenWidth, properties.screenHeight, properties.title.c_str(), nullptr, nullptr);
    log(SUCCESS, "Successfully created GLFW window context!"); 

};

Window::~Window() {
    log(INFO, "Destroying GLFW window"); 
    glfwDestroyWindow(glfwWindow);
    glfwTerminate();
    log(SUCCESS, "Successfully destroyed GLFW window!"); 
};