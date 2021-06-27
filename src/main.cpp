#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "graphics/window.h"
#include "graphics/vulkanInstance.h"
#include "graphics/physicalDevice.h"
#include "graphics/logicalDevice.h"

#include <iostream>

int main() {

    std::cout << "Starting application." << std::endl; 

    Window window;
    VulkanInstance vulkanInstance;
    PhysicalDevice physicalDevice(vulkanInstance.instance);
    LogicalDevice logicalDevice(physicalDevice); 

    while (!glfwWindowShouldClose(window.glfwWindow)) { 
        glfwPollEvents(); 
    }

    std::cout << "Exiting application." << std::endl; 
    return 0;
}