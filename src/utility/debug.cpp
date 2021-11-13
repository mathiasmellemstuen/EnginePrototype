#include "debug.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_vulkan.h>

#include "../graphics/renderer.h"
#include "SDL2/SDL_video.h"
#include "vulkan/vulkan_core.h"
#include <cstddef>
#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iostream>

#define _WIN32_WINNT 0x0A000007
#include <rang/rang.hpp>

VkAllocationCallbacks* Debug::vulkanAllocator = NULL; 
VkInstance Debug::instance = VK_NULL_HANDLE;
VkPhysicalDevice Debug::physicalDevice = VK_NULL_HANDLE;
VkDevice Debug::device = VK_NULL_HANDLE; 
uint32_t Debug::queueFamily = (uint32_t)-1; 
VkQueue Debug::vulkanQueue = VK_NULL_HANDLE; 
VkDebugReportCallbackEXT Debug::debugReport = VK_NULL_HANDLE; 
VkPipelineCache Debug::pipelineCache = VK_NULL_HANDLE; 
VkDescriptorPool Debug::descriptorPool = VK_NULL_HANDLE; 
ImGui_ImplVulkanH_Window Debug::mainWindowData;
uint32_t Debug::minImageCount = 2;
bool Debug::swapChainRebuild = false; 
ImGui_ImplVulkanH_Window* Debug::wd; 
SDL_Window* Debug::debugSdlWindow;
bool Debug::debugWindowRunning = true;

void Debug::log(LogLevel logLevel, std::string message) {
    
    //We will not pring anything if NOTDEBUG is defined. This should in theory also make the compiler remove all the log functions under compiling if NOTDEBUG is defined.
    #ifndef NOTDEBUG

        std::cout << "["; 
        switch (logLevel) {
            case 1:
                std::cout << rang::fg::blue << "INFO" << rang::fg::reset;
            break;
            case 2: 
                std::cout << rang::fg::green << "SUCCESS" << rang::fg::reset; 
            break; 
            case 3:
                std::cout << rang::fg::yellow << "WARNING" << rang::fg::reset; 
            break; 
            case 4: 
                std::cout << rang::fg::red << "ERROR" << rang::fg::reset; 
            break;
            default:
            break; 
        }

        std::cout << "] " << message << std::endl; 
    #endif
};

void Debug::log(LogLevel logLevel, glm::vec2 vec2) {
    Debug::log(logLevel, "X: " + std::to_string(vec2.x) + " Y: " + std::to_string(vec2.y));
};

void Debug::log(LogLevel logLevel, glm::vec3 vec3) {
    Debug::log(logLevel, "X: " + std::to_string(vec3.x) + " Y: " + std::to_string(vec3.y) + " Z: " + std::to_string(vec3.z));
};

void Debug::log(LogLevel logLevel, glm::vec4 vec4) {
   Debug::log(logLevel, "X: " + std::to_string(vec4.x) + " Y: " + std::to_string(vec4.y) + " Z: " + std::to_string(vec4.z) + " W: " + std::to_string(vec4.w));
};

void Debug::log(std::string message) {
    Debug::log(INFO, message); 
};

void Debug::log(glm::vec2 vec2) {
    Debug::log(INFO, vec2); 
};

void Debug::log(glm::vec3 vec3)  {
    Debug::log(INFO, vec3); 
};

void Debug::log(glm::vec4 vec4) {
    Debug::log(INFO, vec4); 
};

void Debug::log(LogLevel logLevel, int value) {
    Debug::log(logLevel, std::to_string(value));
};

void Debug::log(LogLevel logLevel, float value) {
    Debug::log(logLevel, std::to_string(value));
};

void Debug::log(LogLevel logLevel, double value) {
    Debug::log(logLevel, std::to_string(value));
}

void Debug::log(int value) {
    Debug::log(INFO, value); 
};

void Debug::log(float value) {
    Debug::log(INFO, value); 
};

void Debug::log(double value) {
    Debug::log(INFO, value);
}

void Debug::setupDebugWindow() {
    #ifndef NOTDEBUG
        
    Debug::log(INFO, "Creating debug window");

    //TODO: Change value to propertiees values and not hardcoded values.
    debugSdlWindow = SDL_CreateWindow("Engineprototype DEBUG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN |  SDL_WINDOW_RESIZABLE);
    Debug::log(SUCCESS, "Created debug window context."); 

    

    uint32_t extensionsCount = 0;
    SDL_Vulkan_GetInstanceExtensions(debugSdlWindow, &extensionsCount, NULL);
    const char** extensions = new const char*[extensionsCount];
    SDL_Vulkan_GetInstanceExtensions(debugSdlWindow, &extensionsCount, extensions);
    
    //Creating Vulkan instance
    {
        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.enabledExtensionCount = extensionsCount;
        createInfo.ppEnabledExtensionNames = extensions;

        vkCreateInstance(&createInfo, vulkanAllocator, &instance);
    } 

    //Select GPU
    
    uint32_t gpuCount;
    vkEnumeratePhysicalDevices(instance, &gpuCount, NULL);

    VkPhysicalDevice* gpus = (VkPhysicalDevice*)malloc(sizeof(VkPhysicalDevice) * gpuCount);
    vkEnumeratePhysicalDevices(instance, &gpuCount, gpus);

    int useGpu = 0;
    for (int i = 0; i < (int)gpuCount; i++)
    {
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(gpus[i], &properties);
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            useGpu = i;
            break;
        }
    }

    physicalDevice = gpus[useGpu];
    free(gpus);
    
    //Selecting graphics queue family

    uint32_t count;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, NULL);
    VkQueueFamilyProperties* queues = (VkQueueFamilyProperties*)malloc(sizeof(VkQueueFamilyProperties) * count);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, queues);
    for (uint32_t i = 0; i < count; i++)
        if (queues[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFamily = i;
            break;
        }
    free(queues);
    

    // Creating logical device
    {
        int deviceExtensionCount = 1;
        const char* deviceExtensions[] = { "VK_KHR_swapchain" };
        const float queuePriority[] = { 1.0f };
        VkDeviceQueueCreateInfo queueInfo[1] = {};
        queueInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueInfo[0].queueFamilyIndex = queueFamily;
        queueInfo[0].queueCount = 1;
        queueInfo[0].pQueuePriorities = queuePriority;
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = sizeof(queueInfo) / sizeof(queueInfo[0]);
        createInfo.pQueueCreateInfos = queueInfo;
        createInfo.enabledExtensionCount = deviceExtensionCount;
        createInfo.ppEnabledExtensionNames = deviceExtensions;
        vkCreateDevice(physicalDevice, &createInfo, vulkanAllocator, &device);
        vkGetDeviceQueue(device, queueFamily, 0, &vulkanQueue);
    }

    //Creating descriptor pool
    {
        VkDescriptorPoolSize poolSizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
        VkDescriptorPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        poolInfo.maxSets = 1000 * IM_ARRAYSIZE(poolSizes);
        poolInfo.poolSizeCount = (uint32_t)IM_ARRAYSIZE(poolSizes);
        poolInfo.pPoolSizes = poolSizes;
        vkCreateDescriptorPool(device, &poolInfo, vulkanAllocator, &descriptorPool);
    }
    delete[] extensions;

    //Creating the window surface
    VkSurfaceKHR surface;

    if(SDL_Vulkan_CreateSurface(debugSdlWindow, instance, &surface) == 0) {
        //Debug::log(ERROR, "Faile to create Vulkan surface for debug window."); 
    }

    int w, h; 
    SDL_GetWindowSize(debugSdlWindow, &w, &h); 
    wd = &mainWindowData;
    

    wd->Surface = surface; 
    VkBool32 res; 
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily, surface, &res);

    if(res != VK_TRUE) {
        //Debug::log(ERROR, "No WSI support on physical device."); 
    }

    //Selecting surface format
    const VkFormat requestSurfaceImageFormat[] = {VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8_UNORM, VK_FORMAT_R8G8B8_UNORM};


    const VkColorSpaceKHR requestSurfaceColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;

    wd->SurfaceFormat = ImGui_ImplVulkanH_SelectSurfaceFormat(physicalDevice, wd->Surface, requestSurfaceImageFormat, (size_t)IM_ARRAYSIZE(requestSurfaceImageFormat), requestSurfaceColorSpace);

    VkPresentModeKHR presentModes[] = { VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR };

    wd->PresentMode = ImGui_ImplVulkanH_SelectPresentMode(physicalDevice, wd->Surface, &presentModes[0], IM_ARRAYSIZE(presentModes));


    IM_ASSERT(minImageCount >= 2);
    ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, device, wd, queueFamily, vulkanAllocator, w, h, minImageCount);

    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplSDL2_InitForVulkan(debugSdlWindow);
    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = instance;
    initInfo.PhysicalDevice = physicalDevice;
    initInfo.Device = device;
    initInfo.QueueFamily = queueFamily;
    initInfo.Queue = vulkanQueue;
    initInfo.PipelineCache = pipelineCache;
    initInfo.DescriptorPool = descriptorPool;
    initInfo.Allocator = vulkanAllocator;
    initInfo.MinImageCount = minImageCount;
    initInfo.ImageCount = wd->ImageCount;
    //initInfo.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&initInfo, wd->RenderPass);

    
    //Uploading font
    
    {
        // Use any command queue
        VkCommandPool commandPool = wd->Frames[wd->FrameIndex].CommandPool;
        VkCommandBuffer commandBuffer = wd->Frames[wd->FrameIndex].CommandBuffer;

        vkResetCommandPool(device, commandPool, 0);
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);

        VkSubmitInfo endInfo = {};
        endInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        endInfo.commandBufferCount = 1;
        endInfo.pCommandBuffers = &commandBuffer;
        vkEndCommandBuffer(commandBuffer);
        vkQueueSubmit(vulkanQueue, 1, &endInfo, VK_NULL_HANDLE);

        vkDeviceWaitIdle(device);
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }


    #endif
}

void Debug::drawDebugWindow() {
    #ifndef NOTDEBUG
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) {
                Debug::cleanupDebugWindow(); 
                debugWindowRunning = false;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(debugSdlWindow)) {
                Debug::cleanupDebugWindow(); 
                debugWindowRunning = false; 
            }
        }

        // Resize swap chain?
        if (swapChainRebuild) {

            int width, height;
            SDL_GetWindowSize(debugSdlWindow, &width, &height);
            if (width > 0 && height > 0) {

                ImGui_ImplVulkan_SetMinImageCount(minImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(instance, physicalDevice, device, &mainWindowData, queueFamily, vulkanAllocator, width, height, minImageCount);
                mainWindowData.FrameIndex = 0;
                swapChainRebuild = false;
            }
        }
        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Test vindu!"); 
        ImGui::Text("Dette er litt tekst!"); 
        ImGui::End(); 

        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();

        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);
        if (!isMinimized) {

            ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.0f);
            wd->ClearValue.color.float32[0] = clearColor.x * clearColor.w;
            wd->ClearValue.color.float32[1] = clearColor.y * clearColor.w;
            wd->ClearValue.color.float32[2] = clearColor.z * clearColor.w;
            wd->ClearValue.color.float32[3] = clearColor.w;
    
            VkResult err;

            VkSemaphore imageAquiredSemaphore  = wd->FrameSemaphores[wd->SemaphoreIndex].ImageAcquiredSemaphore;

            VkSemaphore renderCompleteSemaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
            
            err = vkAcquireNextImageKHR(device, wd->Swapchain, UINT64_MAX, imageAquiredSemaphore, VK_NULL_HANDLE, &wd->FrameIndex);
            if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR) {
                swapChainRebuild = true;
                return;
            }

            ImGui_ImplVulkanH_Frame* fd = &wd->Frames[wd->FrameIndex];
            {
                vkWaitForFences(device, 1, &fd->Fence, VK_TRUE, UINT64_MAX); // wait indefinitely instead of periodically checking

                vkResetFences(device, 1, &fd->Fence);
            }
            {
                vkResetCommandPool(device, fd->CommandPool, 0);
                VkCommandBufferBeginInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
                info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
                vkBeginCommandBuffer(fd->CommandBuffer, &info);
            }
            {
                VkRenderPassBeginInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
                info.renderPass = wd->RenderPass;
                info.framebuffer = fd->Framebuffer;
                info.renderArea.extent.width = wd->Width;
                info.renderArea.extent.height = wd->Height;
                info.clearValueCount = 1;
                info.pClearValues = &wd->ClearValue;
                vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
            }

            // Record dear imgui primitives into command buffer
            ImGui_ImplVulkan_RenderDrawData(drawData, fd->CommandBuffer);
            // Submit command buffer
            vkCmdEndRenderPass(fd->CommandBuffer);
            {
                VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
                VkSubmitInfo info = {};
                info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
                info.waitSemaphoreCount = 1;
                info.pWaitSemaphores = &imageAquiredSemaphore;
                info.pWaitDstStageMask = &wait_stage;
                info.commandBufferCount = 1;
                info.pCommandBuffers = &fd->CommandBuffer;
                info.signalSemaphoreCount = 1;
                info.pSignalSemaphores = &renderCompleteSemaphore;

                vkEndCommandBuffer(fd->CommandBuffer);
                vkQueueSubmit(vulkanQueue, 1, &info, fd->Fence);
            }

            //Frame present
            if (swapChainRebuild)
                return;
            VkSemaphore render_complete_semaphore = wd->FrameSemaphores[wd->SemaphoreIndex].RenderCompleteSemaphore;
            VkPresentInfoKHR info = {};
            info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
            info.waitSemaphoreCount = 1;
            info.pWaitSemaphores = &render_complete_semaphore;
            info.swapchainCount = 1;
            info.pSwapchains = &wd->Swapchain;
            info.pImageIndices = &wd->FrameIndex;
            err = vkQueuePresentKHR(vulkanQueue, &info);
            if (err == VK_ERROR_OUT_OF_DATE_KHR || err == VK_SUBOPTIMAL_KHR)
            {
                swapChainRebuild = true;
                return;
            }
            wd->SemaphoreIndex = (wd->SemaphoreIndex + 1) % wd->ImageCount; // Now we can use the next set of semaphores
        }
    #endif
}

void Debug::cleanupDebugWindow() {
    #ifndef NOTDEBUG
        vkDeviceWaitIdle(device);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
        ImGui_ImplVulkanH_DestroyWindow(instance, device, &mainWindowData,vulkanAllocator);
        vkDestroyDescriptorPool(device, descriptorPool, vulkanAllocator);
        vkDestroyDevice(device, vulkanAllocator);
        vkDestroyInstance(instance, vulkanAllocator); 
        SDL_DestroyWindow(debugSdlWindow);
        SDL_Quit();
    #endif
}
