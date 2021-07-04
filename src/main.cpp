#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "graphics/window.h"
#include "graphics/vulkanInstance.h"
#include "graphics/physicalDevice.h"
#include "graphics/logicalDevice.h"
#include "graphics/swapChain.h"
#include "utility/properties.h"

#include <iostream>

int main() {
    std::cout << "Starting application." << std::endl; 
    loadPropertiesFromFile();

    Window window;
    VulkanInstance vulkanInstance(*(window.glfwWindow));
    PhysicalDevice physicalDevice(vulkanInstance);
    LogicalDevice logicalDevice(physicalDevice); 
    SwapChain swapChain(physicalDevice, logicalDevice, 800, 600);

    properties->print(); 

    while (!glfwWindowShouldClose(window.glfwWindow)) { 
        glfwPollEvents(); 
    }

    std::cout << "Exiting application." << std::endl; 
    return 0;
}