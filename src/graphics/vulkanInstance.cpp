#include "vulkanInstance.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <stdexcept>
#include "validationLayers.h"
#include "../utility/debug.h"
#include <vector>
#include "renderer.h"

VulkanInstance::VulkanInstance(Renderer& renderer) : renderer(renderer) {

    Debug::log(INFO, "Starting creation of a vulkan instance"); 

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
    SDL_Vulkan_GetInstanceExtensions(renderer.window.sdlWindow, &extensionCount, nullptr);

    std::vector<const char *> extensionNames(extensionCount);
    SDL_Vulkan_GetInstanceExtensions(renderer.window.sdlWindow, &extensionCount, extensionNames.data());

    createInfo.enabledExtensionCount = extensionNames.size(); 
    createInfo.ppEnabledExtensionNames = extensionNames.data(); 

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        Debug::log(ERROR, "Validation layers requested, but not avaiable!"); 
        throw std::runtime_error("Validation layers requested, but not available!"); 
    }

    if (enableValidationLayers) { 
        createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); 
        createInfo.ppEnabledLayerNames = validationLayers.data(); 
    } else { 
        createInfo.enabledLayerCount = 0; 
    }
    
    Debug::log(INFO, "Creating a vulkan instance and VkResult"); 

    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    
    if (result != VK_SUCCESS)  { 
        throw std::runtime_error("Failed to create instance!");
        Debug::log(ERROR, "Failed to create instance!"); 
    }

    if (SDL_Vulkan_CreateSurface(renderer.window.sdlWindow, instance, &surface) == SDL_FALSE) { 
        Debug::log(ERROR, "Failed to create window surface!"); 
        throw std::runtime_error("Failed to create window surface!"); 
    }

    Debug::log(SUCCESS, "Successfully created a vulkan instance"); 
    Debug::log(SUCCESS, "Successfully created a window surface"); 
};

void VulkanInstance::clean() {
    Debug::log(INFO, "Destroying window surface"); 
    Debug::log(INFO, "Destroying vulkan instance"); 
    vkDestroySurfaceKHR(instance, surface, nullptr);
    vkDestroyInstance(instance, nullptr);
    Debug::log(SUCCESS, "Destroyed window surface"); 
    Debug::log(SUCCESS, "Destroyed vulkan instance");
};