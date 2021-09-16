#ifndef ENGINEPROTOTYPE_LOG
#define ENGINEPROTOTYPE_LOG

#include <string>
#include "SDL2/SDL_events.h"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "vulkan/vulkan_core.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_sdl.h>
#include <imgui/backends/imgui_impl_vulkan.h>

class Renderer;

enum LogLevel {
    INFO = 1,
    SUCCESS = 2,
    WARNING = 3,
    ERROR = 4
};

class Debug {
    public:
        static void log(LogLevel logLevel, std::string message);

        static void log(LogLevel logLevel, glm::vec2 vec2);
        static void log(LogLevel logLevel, glm::vec3 vec3); 
        static void log(LogLevel logLevel, glm::vec4 vec4); 

        static void log(LogLevel logLevel, int value);
        static void log(LogLevel logLevel, float value);
        static void log(LogLevel logLevel, double value); 

        static void log(int value); 
        static void log(float value);
        static void log(double value);

        static void log(std::string message); 
        static void log(glm::vec2 vec2); 
        static void log(glm::vec3 vec3); 
        static void log(glm::vec4 vec4); 
        
        static void setupDebugWindow(); 
        static void drawDebugWindow();

        static VkInstance instance;
        static VkAllocationCallbacks* vulkanAllocator;
        static VkPhysicalDevice physicalDevice;
        static VkDevice device;
        static uint32_t queueFamily;
        static VkQueue vulkanQueue;
        static VkDebugReportCallbackEXT debugReport;
        static VkPipelineCache  pipelineCache;
        static VkDescriptorPool descriptorPool;
        static ImGui_ImplVulkanH_Window mainWindowData;
        static uint32_t minImageCount;
        static bool swapChainRebuild;
        static ImGui_ImplVulkanH_Window* wd; 
        static SDL_Window* debugSdlWindow;
};

#endif
