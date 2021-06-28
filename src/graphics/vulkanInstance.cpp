#include "vulkanInstance.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include "validationLayers.h"

VulkanInstance::VulkanInstance(GLFWwindow& window) {

    VkApplicationInfo appInfo{}; 
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
    appInfo.pApplicationName = "Hello Vulkan"; 
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); 
    appInfo.pEngineName = "No Engine"; 
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); 
    appInfo.apiVersion = VK_API_VERSION_1_0; 

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; 
    createInfo.pApplicationInfo = &appInfo;

    uint32_t glfwExtensionCount = 0; 
    const char** glfwExtensions; 

    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); 
    createInfo.enabledExtensionCount = glfwExtensionCount; 
    createInfo.ppEnabledExtensionNames = glfwExtensions;


    if (enableValidationLayers && !checkValidationLayerSupport()) { 
        throw std::runtime_error("Validation layers requested, but not available!"); 
    }

    if (enableValidationLayers) { 
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); 
        createInfo.ppEnabledLayerNames = validationLayers.data(); 
    } else { 
        createInfo.enabledLayerCount = 0; 
    }

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    
    if (result != VK_SUCCESS)  { 
        throw std::runtime_error("Failed to create instance!");
    }

    if (glfwCreateWindowSurface(instance, &window, nullptr, &surface)  != VK_SUCCESS) { 
        throw std::runtime_error("Failed to create window surface!"); 
    }
};

VulkanInstance::~VulkanInstance() {
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
};