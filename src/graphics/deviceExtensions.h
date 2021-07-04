#ifndef ENGINEPROTOTYPE_DEVICEEXTENSIONS
#define ENGINEPROTOTYPE_DEVICEEXTENSIONS

#include <vector>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

#endif