#include "debug.h"
#include "logging.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include <vulkan/vulkan.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include <imgui/imgui_impl_vulkan.h>
#include <imgui/implot.h>
#include <imgui/implot_internal.h>

#include "../graphics/renderer.h"
#include "../input/mouse.h"
#include "../input/keyboardInput.h"

#include "properties.h"

#include "SDL2/SDL_video.h"
#include "vulkan/vulkan_core.h"
#include <cstddef>
#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iostream>

#include "../core/object.h"
#include "../core/component.h"



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
std::vector<float> Debug::recordedFps = {};
Uint32 Debug::lastTime = SDL_GetTicks(); 
Uint32 Debug::currentTime = 0; 
Uint32 Debug::amountOfFrames = 0;
Object* Debug::selectedObject = nullptr;

void Debug::calculateFps() {

    amountOfFrames++;

    if(lastTime < SDL_GetTicks() - 1000) {
        
        lastTime = SDL_GetTicks(); 
        currentTime = amountOfFrames; 
        amountOfFrames = 0; 

        if(currentTime == 0 || currentTime == 1)
            return; 

        recordedFps.push_back((float)currentTime); 
    }
}


void Debug::setupDebugWindow() {
    #ifndef NOTDEBUG


    logger(INFO, "Creating debug window");

    std::string title = (*properties)["windows"]["debug"]["title"];

    int width = (*properties)["windows"]["debug"]["resolution"]["width"];
    int height = (*properties)["windows"]["debug"]["resolution"]["height"];
    debugSdlWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN |  SDL_WINDOW_RESIZABLE);
    
    int xPos = (*properties)["windows"]["debug"]["position"]["x"];
    int yPos = (*properties)["windows"]["debug"]["position"]["y"];
    SDL_SetWindowPosition(debugSdlWindow, xPos, yPos);
    
    logger(SUCCESS, "Created debug window context."); 

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
        logger(INFO, "Failed to create Vulkan surface for debug window."); 
    }

    int w, h; 
    SDL_GetWindowSize(debugSdlWindow, &w, &h); 
    wd = &mainWindowData;
    

    wd->Surface = surface; 
    VkBool32 res; 
    vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamily, surface, &res);

    if(res != VK_TRUE) {
        logger(INFO, "No WSI support on physical device."); 
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
    ImPlot::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.MouseDrawCursor = false;

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

bool Debug::updateDebugWindowEvents(SDL_Event& event) {
    if(event.window.windowID == SDL_GetWindowID(Debug::debugSdlWindow)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        return true;
    }
    return false;
};
void Debug::drawDebugWindow() {
    #ifndef NOTDEBUG

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

        // Drawing the input window
        ImGui::Begin("Input", NULL, ImGuiWindowFlags_AlwaysAutoResize);
        std::string xText = "X: " + std::to_string(Mouse::mousePosition.x);
        std::string yText = "Y: " + std::to_string(Mouse::mousePosition.y);
        ImGui::Text("Mouse Position:");
        ImGui::Text(xText.c_str());
        ImGui::Text(yText.c_str());
        ImGui::Text("Normalized Mouse Position:");
        std::string normalizedXText = "X: " + std::to_string(Mouse::normalizedMousePosition.x); 
        std::string normalizedYText = "Y: " + std::to_string(Mouse::normalizedMousePosition.y);
        ImGui::Text(normalizedXText.c_str());
        ImGui::Text(normalizedYText.c_str());
        ImGui::Text("Mouse Vulkan Coordinate System Screen Position:");
        std::string vulkanScreenXText = "X: " + std::to_string(Mouse::mouseVulkanScreenPosition.x); 
        std::string vulkanScreenYText = "Y: " + std::to_string(Mouse::mouseVulkanScreenPosition.y);
        ImGui::Text(vulkanScreenXText.c_str());
        ImGui::Text(vulkanScreenYText.c_str());
        ImGui::Text("Mouse acceleration:");
        xText = "X: " + std::to_string(Mouse::mouseAcceleration.x);
        yText = "Y: " + std::to_string(Mouse::mouseAcceleration.y);
        ImGui::Text(xText.c_str());
        ImGui::Text(yText.c_str());

    std::string leftMouseButtonPressed = "Mouse button (left) pressed: " + std::to_string(Mouse::mouseLeftIsPressed);
        ImGui::Text(leftMouseButtonPressed.c_str());

        std::string rightMouseButtonPressed = "Mouse button (right) pressed: " + std::to_string(Mouse::mouseRightIsPressed);
        ImGui::Text(rightMouseButtonPressed.c_str());

        std::string pressedKeys = "Keyboard pressed: "; 


        for(int i = 0; i < KeyboardInput::keys.size(); i++) {
            if(KeyboardInput::keys[i].key != "\n") {
                pressedKeys += KeyboardInput::keys[i].key;
                
                if(i < KeyboardInput::keys.size() - 1) {
                    pressedKeys += ", ";
                }
            }
        }
        ImGui::Text(pressedKeys.c_str());
        ImGui::End(); 

        ImGui::Begin("Profiler");

        double xMax = recordedFps.size();
        double yMax = 0; 

        for(float f : recordedFps) {
            yMax = f > yMax ? f : yMax; 
        }
        ImPlot::SetNextAxisLinks(ImAxis_Y1,&yMax, &yMax);
        ImPlot::SetNextAxisLinks(ImAxis_X1,&xMax, &xMax);

        if(ImPlot::BeginPlot("FPS (Frames per second)")) {
            
            float* x = new float[recordedFps.size()];  

            for(int i = 0; i < recordedFps.size(); i++) {
                x[i] = i;
            }

            ImPlot::PlotLine("fps", x, recordedFps.data(), recordedFps.size());

            delete[] x;
            ImPlot::EndPlot();
        }
        ImGui::End();
        ImGui::Begin("Objects");
 
        char buffer[255]{};
        ImGui::InputText("search", buffer, sizeof(buffer));
        std::string input(buffer);

        for(Object* object : Object::objects) {
            
            std::string title = std::string(object->name + " #" + std::to_string(object->id)).c_str();

            if(ImGui::Button(title.c_str())) {
                Debug::selectedObject = object;
            }

        }
        ImVec2 size = ImGui::GetWindowSize();
        ImVec2 pos = ImGui::GetWindowPos();

        ImGui::End();
        Debug::drawSelectedObject(pos, size);

        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();

        const bool isMinimized = (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f);

        if (!isMinimized) {
            ImVec4 clearColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
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

void Debug::drawSelectedObject(ImVec2 pos, ImVec2 size) {

    bool closeSelectedObject = false;

    if(Debug::selectedObject != nullptr) {
        ImGui::SetNextWindowSize(size);
        ImGui::SetNextWindowPos(pos);
        ImGui::Begin("Object");

        if(ImGui::Button("X")) {
            closeSelectedObject = true;
        }
        Debug::selectedObject->debug();

        ImGui::End();
    }

    if(closeSelectedObject) {
        Debug::selectedObject = nullptr;
    }
};
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
    #endif
}
