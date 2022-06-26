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
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_vulkan.h>
#include <vector>

#include "../core/object.h"

class Renderer;

class Debug {
    public:
        static void setupDebugWindow(); 
        static void drawDebugWindow();
        static bool updateDebugWindowEvents(SDL_Event& event);
        static void cleanupDebugWindow();
        static void calculateFps(); 
        static void drawSelectedObject(ImVec2 pos, ImVec2 size);

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
        static bool debugWindowRunning;
        static std::vector<float> recordedFps;
        static Uint32 lastTime;
        static Uint32 currentTime;
        static Uint32 amountOfFrames;  
        static Object* selectedObject;
};

#endif
