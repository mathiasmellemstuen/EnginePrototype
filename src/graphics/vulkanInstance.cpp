#include "vulkanInstance.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include "validationLayers.h"
#include "../utility/log.h"
#include <vector>

VulkanInstance::VulkanInstance(SDL_Window& window) {

    log(INFO, "Starting creation of a vulkan instance"); 

    VkApplicationInfo appInfo{}; 
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; 
    appInfo.pApplicationName = "Vulkan tutorial"; // TODO: Change this to app title properties. 
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); 
    appInfo.pEngineName = "No Engine"; 
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); 
    appInfo.apiVersion = VK_API_VERSION_1_0; 

    VkInstanceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; 
    createInfo.pApplicationInfo = &appInfo;

    unsigned int extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(&window, &extensionCount, nullptr);

    std::vector<const char *> extensionNames(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(&window, &extensionCount, extensionNames.data());

    createInfo.enabledExtensionCount = extensionNames.size(); 
    createInfo.ppEnabledExtensionNames = extensionNames.data(); 

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        log(ERROR, "Validation layers requested, but not avaiable!"); 
        throw std::runtime_error("Validation layers requested, but not available!"); 
    }

    if (enableValidationLayers) { 
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); 
        createInfo.ppEnabledLayerNames = validationLayers.data(); 
    } else { 
        createInfo.enabledLayerCount = 0; 
    }
    
    log(INFO, "Creating a vulkan instance and VkResult"); 

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    
    if (result != VK_SUCCESS)  { 
        throw std::runtime_error("Failed to create instance!");
        log(ERROR, "Failed to create instance!"); 
    }

    if (SDL_Vulkan_CreateSurface(&window, instance, &surface) == SDL_FALSE) { 
        log(ERROR, "Failed to create window surface!"); 
        throw std::runtime_error("Failed to create window surface!"); 
    }

    log(SUCCESS, "Successfully created a vulkan instance"); 
    log(SUCCESS, "Successfully created a window surface"); 
};

VulkanInstance::~VulkanInstance() {
    log(INFO, "Destroying window surface"); 
    log(INFO, "Destroying vulkan instance"); 
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    log(SUCCESS, "Destroyed window surface"); 
    log(SUCCESS, "Destroyed vulkan instance");
};