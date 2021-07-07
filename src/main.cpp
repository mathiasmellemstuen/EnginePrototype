#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "graphics/window.h"
#include "graphics/vulkanInstance.h"
#include "graphics/physicalDevice.h"
#include "graphics/logicalDevice.h"
#include "graphics/swapChain.h"
#include "utility/properties.h"
#include "graphics/imageViews.h"
#include "graphics/shader.h"
#include "utility/log.h"

#include <iostream>

int main() {
    std::cout << "Starting application." << std::endl; 
    log(INFO, "Starting application."); 

    loadPropertiesFromFile();
    printProperties();

    Window window;
    VulkanInstance vulkanInstance(*(window.glfwWindow));
    PhysicalDevice physicalDevice(vulkanInstance);
    LogicalDevice logicalDevice(physicalDevice); 
    SwapChain swapChain(physicalDevice, logicalDevice);
    ImageViews ImageViews(swapChain, logicalDevice);
    Shader shader(logicalDevice, "shaders/vert.spv", "shaders/frag.spv");

    while (!glfwWindowShouldClose(window.glfwWindow)) { 
        glfwPollEvents(); 
    }

    log(INFO, "Exiting application!"); 
    return 0;
}