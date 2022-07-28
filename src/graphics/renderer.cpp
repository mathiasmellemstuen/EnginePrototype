#include "renderer.h"
#include "window.h"
#include <vulkan/vulkan.h>
#include "../utility/logging.h"
#include "../utility/debug.h"
#include "validationLayers.h"
#include "QueueFamilyIndices.h"
#include <set>
#include "deviceExtensions.h"
#include "swapChainSupport.h"
#include "camera.h"
#include "eventManager.h"
#include "../core/object.h"
#include "graphicsEntityInstance.h"
#include "genericGraphicsEntityInstance.h"
#include <string>
#include "UI/UIInstance.h"
#include "camera.h"

uint64_t last = 0; 
uint64_t now = 0; 

void createCommandBuffersInlineContentForSingleRenderPass(Renderer& renderer, uint32_t currentImage, RenderPassObject& renderPassObject) {
    std::vector<GenericGraphicsEntityInstance*> instances;
    
    for(Object* object : Object::objects) {
        GenericGraphicsEntityInstance* inst = object->getComponent<GenericGraphicsEntityInstance>();
        
        if(inst != nullptr)
            instances.push_back(inst);
    }
    
    // Sorting the list if it is layered
    if(renderPassObject.usingLayers) {
        struct {
            bool operator()(GenericGraphicsEntityInstance* a, GenericGraphicsEntityInstance* b) const {return a->layer < b->layer; }
        } custom;

        std::sort(instances.begin(), instances.end(), custom);
    }
   
    for(GenericGraphicsEntityInstance* instance : instances) {
        instance->render(renderer, currentImage);
    }
}

// Changed from create to ->
void createCommandBuffers(Renderer& renderer, uint32_t currentImage) {
    
    for (size_t i = 0; i < renderer.commandBuffers.size(); i++) {
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(renderer.commandBuffers[i], &beginInfo) != VK_SUCCESS) { 
            logger(ERROR, "Failed to begin recording command buffers!"); 
            throw std::runtime_error("failed to begin recording command buffer!");
        } 
        
        for(const auto& [key, value] : renderer.renderPasses) {
            logger(INFO, "Starting " + (std::string)key);
            vkCmdBeginRenderPass(renderer.commandBuffers[i], &value.beginInfos[i], VK_SUBPASS_CONTENTS_INLINE);
            logger(INFO, key);
            logger(INFO, (int)value.beginInfos[i].clearValueCount);
            createCommandBuffersInlineContentForSingleRenderPass(renderer, i, (RenderPassObject&)value);

            logger(INFO, "Ending");
            vkCmdEndRenderPass(renderer.commandBuffers[i]);
            logger(INFO, "Ending done.");
        }
        
        if (vkEndCommandBuffer(renderer.commandBuffers[i]) != VK_SUCCESS) {
            logger(ERROR, "Failed to record command buffer!");
            throw std::runtime_error("failed to record command buffer!");
        }
    }
        /*
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPassContent.renderPass;
        renderPassInfo.framebuffer = renderPassContent.frameBuffers[i];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = renderer.swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        //TODO: Change rendering of instances to its own rendering virtual function in GenericGraphicsEntityInstance

        vkCmdBeginRenderPass(renderer.commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            
            for(Object* object : Object::objects) {
                // Rendering normal graphics entity instances
                GraphicsEntityInstance<UniformBufferObject>* currentGraphicsEntityInstance = object->getComponent<GraphicsEntityInstance<UniformBufferObject>>();
                Transform* currentTransform = object->getComponent<Transform>(); 

                if(currentGraphicsEntityInstance != nullptr && currentTransform != nullptr) {
                    currentGraphicsEntityInstance->uniformBufferObject = {Camera::mainCamera->view, Camera::mainCamera->projection, currentTransform->getModel()};
                    currentGraphicsEntityInstance->render(renderer, i);
                    continue; 
                }
                // Making sure we are not rendering user interface objects
                UIInstance* currentUIInstance = object->getComponent<UIInstance>();

                if(currentUIInstance != nullptr) {
                    continue;
                }

                // TODO: Rendering all custom graphics entities

            }
        
        vkCmdEndRenderPass(renderer.commandBuffers[i]);
        
        // Setting up UI render pass
        VkRenderPassBeginInfo uiRenderPassInfo{};
        uiRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        uiRenderPassInfo.renderPass = renderer.uiRenderPass;
        uiRenderPassInfo.framebuffer = renderer.swapChainFramebuffers[i];
        uiRenderPassInfo.renderArea.offset = {0, 0};
        uiRenderPassInfo.renderArea.extent = renderer.swapChainExtent;
        uiRenderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        uiRenderPassInfo.pClearValues = clearValues.data();

        // Starting UI render pass
        vkCmdBeginRenderPass(renderer.commandBuffers[i], &uiRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            std::vector<UIInstance*> instances; 

            for(Object* object : Object::objects) {
                
                // Rendering all user interface objects!
                UIInstance* currentUIInstance = object->getComponent<UIInstance>();

                if(currentUIInstance != nullptr) {
                    instances.push_back(currentUIInstance);
                    continue;
                }
            }

            struct {
                bool operator()(UIInstance* a, UIInstance* b) const {return a->layer < b->layer; }
            } custom;

            std::sort(instances.begin(), instances.end(), custom); 

            for(UIInstance* instance : instances) {
                instance->render(renderer, i);
            }

        vkCmdEndRenderPass(renderer.commandBuffers[i]);

        */

}

uint32_t findMemoryType(Renderer& renderer, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(renderer.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
};

void createImage(Renderer& renderer, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {

    logger(INFO, "Creating vulkan image");

    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = numSamples;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(renderer.device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        logger(ERROR, "Failed to create image!"); 
        throw std::runtime_error("failed to create image!");
    }
    logger(SUCCESS, "Succesfully created image!"); 

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(renderer.device, image, &memRequirements);
    
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(renderer, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(renderer.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }
    vkBindImageMemory(renderer.device, image, imageMemory, 0);

    logger(SUCCESS, "Succesfsfully created vulkan image!"); 
}
VkFormat findSupportedFormat(Renderer& renderer, const std::vector<VkFormat>&candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    
    for (VkFormat format : candidates) {

        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(renderer.physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    logger(ERROR, "Failed to find supported format!"); 
    throw std::runtime_error("failed to find supported format!"); 
}
VkFormat findDepthFormat(Renderer& renderer) {
    return findSupportedFormat(renderer, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
VkImageView createImageView(Renderer& renderer, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;

    if (vkCreateImageView(renderer.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}
VkSampleCountFlagBits getMaxUsableSampleCount(Renderer& renderer) {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(renderer.physicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}

bool checkDeviceExtensionSupport(Renderer& renderer) {
    
    uint32_t extensionCount; 
    vkEnumerateDeviceExtensionProperties(renderer.physicalDevice, nullptr, &extensionCount, nullptr); 
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount); 
    vkEnumerateDeviceExtensionProperties(renderer.physicalDevice, nullptr, &extensionCount, availableExtensions.data()); 
    
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end()); 
    
    for (const auto& extension : availableExtensions) { 
        requiredExtensions.erase(extension.extensionName); 
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices findQueueFamilies(Renderer& renderer) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(renderer.physicalDevice, &queueFamilyCount,  nullptr); 
 
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount); 
    vkGetPhysicalDeviceQueueFamilyProperties(renderer.physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {

        if (indices.isComplete()) {
            break;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { 
            indices.graphicsFamily = i; 
        }

        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(renderer.physicalDevice, i, renderer.surface, &presentSupport);
         
        if (presentSupport) {
            indices.presentFamily = i;
        }

        i++; 
    }
    return indices; 
}

bool isDeviceSuitable(Renderer& renderer, const VkPhysicalDevice& device) {
    
    logger(INFO, "Checking if device is suitable."); 
    renderer.physicalDevice = device;

    QueueFamilyIndices indices = findQueueFamilies(renderer); 
    bool extensionsSupported = checkDeviceExtensionSupport(renderer); 

    bool swapChainAdequate = false; 

    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport =  querySwapChainSupport(device, renderer.surface); 
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty(); 
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);


    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy; 
};

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

    for (const auto& availableFormat : availableFormats) { 
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&  availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) { 
            return availableFormat; 
        } 
    }

    return availableFormats[0]; 
};

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) { 
    
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode; 
        }
    } 

    return VK_PRESENT_MODE_FIFO_KHR;
};

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window& window) { 

    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    int width, height;
    SDL_GetWindowSize(window.sdlWindow, &width, &height);
    VkExtent2D actualExtent = {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width)); 
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height)); 

    return actualExtent;
};

void cleanupSwapChain(Renderer& renderer) {

    logger(INFO, "Starting cleanup of swap chain");
    
    for(const auto& [key, value] : renderer.renderPasses) {
        if(!value.usingSharedResources) {
            for (size_t i = 0; i < value.frameBuffers.size(); i++)
                vkDestroyFramebuffer(renderer.device, value.frameBuffers[i], nullptr);
        }
        
        vkDestroyRenderPass(renderer.device, value.renderPass, nullptr);
        
    }
    
    for(int i = allGraphicsEntities.size() - 1; i < 0; i++) {
        freeGraphicsEntity(renderer, *allGraphicsEntities[i]);
    }

    for (size_t i = 0; i < renderer.swapChainImageViews.size(); i++) {
        vkDestroyImageView(renderer.device, renderer.swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(renderer.device, renderer.swapChain, nullptr);

    logger(SUCCESS, "Successfully cleaned up swapchain"); 
};
void reCreateSwapChain(Renderer& renderer, Window& window) {

    if(!window.running)
        return;

    logger(INFO, "Recreating the swapchain!"); 

    int width = 0, height = 0;
    SDL_GetWindowSize(window.sdlWindow, &width, &height);

    while ((width == 0 || height == 0) && window.running) {
        SDL_GetWindowSize(window.sdlWindow, &width, &height);
        SDL_Delay(1); 
    }
    // Changing the aspect ratio of the main camera
    Camera::mainCamera->aspectRatio = (float)width / (float) height; 

    vkDeviceWaitIdle(renderer.device);

    cleanupSwapChain(renderer); 
    allocateSwapchainDependentRendererContent(renderer, window); 

    for(GraphicsEntity* graphicsEntity : allGraphicsEntities) {
        reCreateGraphicsEntity(renderer, *graphicsEntity);
    }

    for(Object* object : Object::objects) {
        GenericGraphicsEntityInstance* instance = object->getComponent<GenericGraphicsEntityInstance>(); 

        if(instance != nullptr) {
            instance->reCreateGraphics(renderer);
        }
    }
    // TODO: Re dreate / Allocate command buffers here!

    logger(SUCCESS, "Done recreating swapchain!"); 
}
void drawFrame(Renderer& renderer, Window& window) {

    vkWaitForFences(renderer.device, 1, &renderer.inFlightFences[renderer.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(renderer.device, renderer.swapChain, UINT64_MAX, renderer.imageAvailableSemaphores[renderer.currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        reCreateSwapChain(renderer, window);
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    if (renderer.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(renderer.device, 1, &renderer.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    renderer.imagesInFlight[imageIndex] = renderer.inFlightFences[renderer.currentFrame];

   // uniformBuffer.update(imageIndex); 
    createCommandBuffers(renderer, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {renderer.imageAvailableSemaphores[renderer.currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &renderer.commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {renderer.renderFinishedSemaphores[renderer.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(renderer.device, 1, & renderer.inFlightFences[renderer.currentFrame]);

    if (vkQueueSubmit(renderer.graphicsQueue, 1, &submitInfo, renderer.inFlightFences[renderer.currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {renderer.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(renderer.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.framebufferResized) {

        window.framebufferResized = false;
        reCreateSwapChain(renderer, window); 

    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    //TODO: Change 2 to properties.maxFramesInFlight when yamlparser is done. 
    renderer.currentFrame = (renderer.currentFrame + 1) % 2;
}
void loop(Renderer& renderer, Window& window, EventManager& eventManager) {

    now = SDL_GetPerformanceCounter(); 
    last = 0; 

    createCommandBuffers(renderer, 0); 

    while(window.running) {
        // Calculating delta time
        last = now; 
        now = SDL_GetPerformanceCounter(); 
        float deltaTime = (float)((now - last) * 1000 / (float)SDL_GetPerformanceFrequency()) / 1000.0f;

        #ifndef NO_DEBUG_WINDOW
        // For debug window
        Debug::calculateFps(); 
        Debug::drawDebugWindow();
        #endif

        eventManager.update(window);

        for(Object* object : Object::objects) {
            object->update(deltaTime);
        }

        drawFrame(renderer, window);
        vkDeviceWaitIdle(renderer.device);
    }
};

void allocateSwapchainDependentRendererContent(Renderer& renderer, Window& window) {
    {
        logger(INFO, "Setting up swap chain"); 

        SwapChainSupportDetails swapChainSupport =  querySwapChainSupport(renderer.physicalDevice, renderer.surface); 

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode =  chooseSwapPresentMode(swapChainSupport.presentModes); 
        VkExtent2D extent =  chooseSwapExtent(swapChainSupport.capabilities, window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount >  swapChainSupport.capabilities.maxImageCount) { 
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = renderer.surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(renderer);
        uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()}; 

        if (indices.graphicsFamily != indices.presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else { 
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional 
        }

        createInfo.preTransform =  swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(renderer.device, &createInfo, nullptr, &renderer.swapChain)  != VK_SUCCESS) { 
            logger(ERROR, "Failed to create swap chain!"); 
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(renderer.device, renderer.swapChain, &imageCount, nullptr);
        renderer.swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(renderer.device, renderer.swapChain, &imageCount, renderer.swapChainImages.data());

        renderer.swapChainImageFormat = surfaceFormat.format;
        renderer.swapChainExtent = extent;

        logger(SUCCESS, "Created swap chain"); 
    }
    {
        logger(INFO, "Create image views");

        renderer.swapChainImageViews.resize(renderer.swapChainImages.size());

        for (uint32_t i = 0; i < renderer.swapChainImages.size(); i++) {
            renderer.swapChainImageViews[i] = createImageView(renderer, renderer.swapChainImages[i], renderer.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }

        logger(SUCCESS, "Image views created!"); 
    }
    {

        logger(INFO, "Creating render pass"); 
        
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = findDepthFormat(renderer);
        depthAttachment.samples = renderer.msaaSamples;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = renderer.swapChainImageFormat; 
        colorAttachment.samples = renderer.msaaSamples;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0; 
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = renderer.swapChainImageFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Changing this to go to the next render pass instead of present src khr

        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 2;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;
        
        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;
       
        std::vector<VkClearValue> clearValues;
        clearValues.resize(2);
        clearValues[0].color = {135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};


        std::vector<ImageData> renderPassImageAttachments = {
                createAttachment(renderer, renderer.swapChainImageFormat, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_ASPECT_COLOR_BIT),
                createAttachment(renderer, findDepthFormat(renderer), VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_IMAGE_ASPECT_DEPTH_BIT)
        };

        createRenderPass(renderer, "world", renderPassInfo, clearValues, nullptr, &renderPassImageAttachments, false);

        logger(SUCCESS, "successfully created render pass!");

    }
    {
        logger(INFO, "Creating UI render pass!");

        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = renderer.swapChainImageFormat; 
        colorAttachment.samples = renderer.msaaSamples;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // Changing this to not have clear color
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Changing this because we have a previous render pass
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; 

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0; 
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = findDepthFormat(renderer);
        depthAttachment.samples = renderer.msaaSamples;
        depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentReference depthAttachmentRef{};
        depthAttachmentRef.attachment = 1;
        depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

        VkAttachmentDescription colorAttachmentResolve{};
        colorAttachmentResolve.format = renderer.swapChainImageFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD; // Changing this to not have clear color
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; // Changing this because we have a previous render pass
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        
        VkAttachmentReference colorAttachmentResolveRef{};
        colorAttachmentResolveRef.attachment = 2;
        colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        std::array<VkAttachmentDescription, 3> attachments = {colorAttachment, depthAttachment, colorAttachmentResolve};
        
        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;
        subpass.pDepthStencilAttachment = &depthAttachmentRef;
        subpass.pResolveAttachments = &colorAttachmentResolveRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        renderPassInfo.pAttachments = attachments.data();
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;


        createRenderPass(renderer, "UI", renderPassInfo, {}, &renderer.renderPasses.at("world"), nullptr, true);
        logger(SUCCESS, "Created UI render pass!");
    }
}
const Renderer& createRenderer(Window& window) {
    Renderer* renderer = new Renderer;

    {
        logger(INFO, "Starting creation of a vulkan instance"); 

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
        SDL_Vulkan_GetInstanceExtensions(window.sdlWindow, &extensionCount, nullptr);

        std::vector<const char *> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window.sdlWindow, &extensionCount, extensionNames.data());

        createInfo.enabledExtensionCount = extensionNames.size(); 
        createInfo.ppEnabledExtensionNames = extensionNames.data();

        if (enableValidationLayers && !checkValidationLayerSupport()) {
            logger(ERROR, "Validation layers requested, but not avaiable!"); 
            throw std::runtime_error("Validation layers requested, but not available!"); 
        }

        if (enableValidationLayers) { 
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size()); 
            createInfo.ppEnabledLayerNames = validationLayers.data(); 
        } else { 
            createInfo.enabledLayerCount = 0; 
        }
        
        logger(INFO, "Creating a vulkan instance and VkResult"); 

        VkResult result = vkCreateInstance(&createInfo, nullptr, &renderer->instance);
        
        if (result != VK_SUCCESS)  { 
            throw std::runtime_error("Failed to create instance!");
            logger(ERROR, "Failed to create instance!"); 
        }

        if (SDL_Vulkan_CreateSurface(window.sdlWindow, renderer->instance, &renderer->surface) == SDL_FALSE) { 
            logger(ERROR, "Failed to create window surface!"); 
            throw std::runtime_error("Failed to create window surface!"); 
        }

        logger(SUCCESS, "Successfully created a vulkan instance"); 
        logger(SUCCESS, "Successfully created a window surface"); 
    }
    {
        logger(INFO, "Checking and creating physical device context"); 

        uint32_t deviceCount = 0; 
        vkEnumeratePhysicalDevices(renderer->instance, &deviceCount, nullptr);

        if (deviceCount == 0) { 
            logger(ERROR, "Failed to find GPUs with Vulkan support!"); 
            throw std::runtime_error("Failed to find GPUs with Vulkan support!"); 
        }

        std::vector<VkPhysicalDevice> devices(deviceCount); 
        vkEnumeratePhysicalDevices(renderer->instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(*renderer, device)) { 
                renderer->physicalDevice = device;
                renderer->msaaSamples = getMaxUsableSampleCount(*renderer);
                break; 
            }
        }

        if (renderer->physicalDevice == VK_NULL_HANDLE) { 
            
            logger(ERROR, "Failed to find a suitable GPU!"); 
            throw std::runtime_error("Failed to find a suitable GPU!");
    
        } else {
        
            vkGetPhysicalDeviceProperties(renderer->physicalDevice,&renderer->physicalDeviceProperties);
            logger(SUCCESS, "Selected GPU: " + std::string(renderer->physicalDeviceProperties.deviceName) + ". Successfully created PhysicalDevice context.");
        }
    }
    {
        logger(INFO, "Setting up a logical device");

        QueueFamilyIndices indices = findQueueFamilies(*renderer);

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
        queueCreateInfo.queueCount = 1;

        float queuePriority = 1.0f;

        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType =  VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};
        deviceFeatures.samplerAnisotropy = VK_TRUE;
        //TODO: Find a way to check for geometry shader support since it's not supported on macs? 
        //deviceFeatures.geometryShader = VK_TRUE;

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = &queueCreateInfo;
        createInfo.queueCreateInfoCount = 1;
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount =  static_cast<uint32_t>(deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = deviceExtensions.data();
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        if (enableValidationLayers) {
            createInfo.enabledLayerCount =
                    static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data(); 
        } else { 
            createInfo.enabledLayerCount = 0; 
        }

        if (vkCreateDevice(renderer->physicalDevice, &createInfo, nullptr, &renderer->device) !=  VK_SUCCESS) { 
            
            logger(ERROR, "Failed to create logical device!"); 
            throw std::runtime_error("Failed to create logical device!"); 
        }

        vkGetDeviceQueue(renderer->device, indices.graphicsFamily.value(), 0, &renderer->graphicsQueue);
        vkGetDeviceQueue(renderer->device, indices.presentFamily.value(), 0, &renderer->presentQueue);

        logger(SUCCESS, "Created logical device!"); 
    }
    allocateSwapchainDependentRendererContent(*renderer, window);
    {
        logger(INFO, "Creating command pool."); 

        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(*renderer);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(); 
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(renderer->device, &poolInfo, nullptr, &renderer->commandPool) != VK_SUCCESS) {
            logger(ERROR, "Failed to create command pool!"); 
            throw std::runtime_error("failed to create command pool!");
        }
        logger(SUCCESS, "Command pool created!"); 
    }
    {

        logger(INFO, "Allocating command buffers");

        renderer->commandBuffers.resize(renderer->swapChainImages.size());
        
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = renderer->commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) renderer->commandBuffers.size();

        if (vkAllocateCommandBuffers(renderer->device, &allocInfo, renderer->commandBuffers.data()) != VK_SUCCESS) {
            logger(ERROR, "Failed to allocate command buffers!"); 
            throw std::runtime_error("failed to allocate command buffers!"); 
        }

        logger(SUCCESS, "Command buffers successfully allocated!");
    }
    {

        logger(INFO, "Creating sync objects"); 

        //TODO: Change 2 with max frames in flight variable from properties. 
        renderer->imageAvailableSemaphores.resize(2);
        renderer->renderFinishedSemaphores.resize(2);
        renderer->imagesInFlight.resize(renderer->swapChainImages.size(), VK_NULL_HANDLE);
        renderer->inFlightFences.resize(2);


        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        //TODO: Change 2 with max frames in flight variable from properties. 
        for (size_t i = 0; i < 2; i++) {
            
            if (vkCreateSemaphore(renderer->device, &semaphoreInfo, nullptr, &renderer->imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(renderer->device, &semaphoreInfo, nullptr, &renderer->renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(renderer->device, &fenceInfo, nullptr, &renderer->inFlightFences[i]) != VK_SUCCESS) {
                
                logger(ERROR, "Failed to create synchronization objects for a frame!"); 
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }

        logger(SUCCESS, "Successfully created sync objects!");
    }
    return *renderer;
}

void destroyRenderer(Renderer& renderer) {

    //TODO: Change 2 with max frames in flight variable from properties.
    for (size_t i = 0; i < 2; i++) {
        vkDestroySemaphore(renderer.device, renderer.renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(renderer.device, renderer.imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(renderer.device, renderer.inFlightFences[i], nullptr);
    }

    for(const auto& [key, value] : renderer.renderPasses) {
        if(value.usingSharedResources == false) {
            for(int i = 0; i < value.frameBuffers.size(); i++) {
                vkDestroyFramebuffer(renderer.device, value.frameBuffers[i], nullptr);
            }
            for(int i = 0; i < value.attachments->size(); i++) {
                vkDestroyImageView(renderer.device, value.attachments->at(i).view, nullptr);
                vkDestroyImage(renderer.device, value.attachments->at(i).image, nullptr);
                vkFreeMemory(renderer.device, value.attachments->at(i).memory, nullptr);
            }
        }
    }

    for (auto imageView : renderer.swapChainImageViews) {
        vkDestroyImageView(renderer.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(renderer.device, renderer.swapChain, nullptr);
    vkDestroyDevice(renderer.device, nullptr);
    vkDestroySurfaceKHR(renderer.instance, renderer.surface, nullptr);
    vkDestroyInstance(renderer.instance, nullptr);

    delete &renderer; 
}


void createBuffer(Renderer& renderer, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(renderer.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(renderer.device, buffer, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
    findMemoryType(renderer, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(renderer.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(renderer.device, buffer, bufferMemory, 0);
}

VkCommandBuffer beginSingleTimeCommands(Renderer& renderer) {
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = renderer.commandPool;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(renderer.device, &allocInfo, &commandBuffer);
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    return commandBuffer;
}
void endSingleTimeCommands(Renderer& renderer, VkCommandBuffer commandBuffer) {

    vkEndCommandBuffer(commandBuffer);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(renderer.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(renderer.graphicsQueue);

    vkFreeCommandBuffers(renderer.device, renderer.commandPool, 1, &commandBuffer);
}
bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}
void generateMipmaps(Renderer& renderer, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(renderer.physicalDevice, imageFormat, &formatProperties);
    
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(renderer);

        VkImageMemoryBarrier barrier{};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.image = image;
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;
        barrier.subresourceRange.levelCount = 1;

        int32_t mipWidth = texWidth;
        int32_t mipHeight = texHeight;

        for (uint32_t i = 1; i < mipLevels; i++) {

            barrier.subresourceRange.baseMipLevel = i - 1;
            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            VkImageBlit blit{};
            blit.srcOffsets[0] = {0, 0, 0};
            blit.srcOffsets[1] = {mipWidth, mipHeight, 1};
            blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.srcSubresource.mipLevel = i - 1;
            blit.srcSubresource.baseArrayLayer = 0;
            blit.srcSubresource.layerCount = 1;
            blit.dstOffsets[0] = {0, 0, 0};
            blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
            blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            blit.dstSubresource.mipLevel = i;
            blit.dstSubresource.baseArrayLayer = 0;
            blit.dstSubresource.layerCount = 1;

            vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

            barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            if (mipWidth > 1) mipWidth /= 2;
            if (mipHeight > 1) mipHeight /= 2;
        }

        barrier.subresourceRange.baseMipLevel = mipLevels - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);
    endSingleTimeCommands(renderer, commandBuffer);
}
void copyBuffer(Renderer& renderer, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(renderer);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(renderer, commandBuffer);
}



void createRenderPass(Renderer& renderer, const char* renderPassName, VkRenderPassCreateInfo& createInfo, std::vector<VkClearValue> clearValues, RenderPassObject* sharedResources, std::vector<ImageData>* attachments, bool usingLayers) {

    logger(INFO, "Creating renderpass: " + (std::string)renderPassName);

    RenderPassObject* renderPassObject = new RenderPassObject;
    renderPassObject->createInfo = createInfo;
    renderPassObject->usingLayers = usingLayers;
    renderPassObject->usingSharedResources = sharedResources == nullptr  ? false : true;

    if (vkCreateRenderPass(renderer.device, &renderPassObject->createInfo, nullptr, &renderPassObject->renderPass) != VK_SUCCESS) {
        logger(ERROR, "Failed to create render pass!");
        throw std::runtime_error("failed to create render pass!");
    }

    if(renderPassObject->usingSharedResources == true) {
        renderPassObject->attachments = sharedResources->attachments;
        renderPassObject->frameBuffers = sharedResources->frameBuffers;
        renderPassObject->beginInfos = sharedResources->beginInfos;
    } else {
        renderPassObject->attachments = attachments;
        renderPassObject->frameBuffers.resize(renderer.swapChainImages.size());
        renderPassObject->beginInfos.resize(renderer.swapChainImages.size());

        for (size_t i = 0; i < renderPassObject->frameBuffers.size(); i++) {

            std::vector<VkImageView> finalAttachments;
            finalAttachments.resize(renderPassObject->attachments->size() + 1);

            for (int j = 0; j < renderPassObject->attachments->size(); j++) {
                finalAttachments[j] = renderPassObject->attachments->at(j).view;
            }
            finalAttachments[finalAttachments.size() - 1] = renderer.swapChainImageViews[i];
            //finalAttachments[renderPassObject->attachments->size()] = renderer.swapChainImageViews[i];

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPassObject->renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(finalAttachments.size());
            framebufferInfo.pAttachments = finalAttachments.data();
            framebufferInfo.width = renderer.swapChainExtent.width;
            framebufferInfo.height = renderer.swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(renderer.device, &framebufferInfo, nullptr, &renderPassObject->frameBuffers[i]) !=
                VK_SUCCESS) {
                logger(ERROR, "Failed to create framebuffer!");
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
    }

    for(int i = 0; i < renderPassObject->frameBuffers.size(); i++) {
        renderPassObject->beginInfos[i].sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassObject->beginInfos[i].renderPass = renderPassObject->renderPass;
        renderPassObject->beginInfos[i].framebuffer = renderPassObject->frameBuffers[i];
        renderPassObject->beginInfos[i].renderArea.offset = {0, 0};
        renderPassObject->beginInfos[i].renderArea.extent = renderer.swapChainExtent;
        renderPassObject->beginInfos[i].clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassObject->beginInfos[i].pClearValues = clearValues.size() == 0 ? nullptr : clearValues.data();
    }
    renderer.renderPasses.insert(std::pair<const char*, RenderPassObject>(renderPassName, *renderPassObject));
}
const ImageData& createAttachment(Renderer& renderer, VkFormat format, VkImageUsageFlags usage, VkImageAspectFlagBits aspectFlags) {
    ImageData* imageData = new ImageData; 
    
    createImage(renderer, renderer.swapChainExtent.width, renderer.swapChainExtent.height, 1, renderer.msaaSamples, format, VK_IMAGE_TILING_OPTIMAL, usage, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, imageData->image, imageData->memory);
    imageData->view = createImageView(renderer, imageData->image, format, aspectFlags, 1);

    return *imageData;
}
