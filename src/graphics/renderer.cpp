#include "renderer.h"
#include "window.h"
#include <vulkan/vulkan.h>
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
#include <string>
#include "UI/UIInstance.h"
uint64_t last = 0; 
uint64_t now = 0; 

// Changed from create to ->
void createCommandBuffers(RendererContent& rendererContent, uint32_t currentImage) {
    
    for (size_t i = 0; i < rendererContent.commandBuffers.size(); i++) {
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    

        if (vkBeginCommandBuffer(rendererContent.commandBuffers[i], &beginInfo) != VK_SUCCESS) { 
            Debug::log(ERROR, "Failed to begin recording command buffers!"); 
            throw std::runtime_error("failed to begin recording command buffer!");
        } 
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = rendererContent.renderPass;
        renderPassInfo.framebuffer = rendererContent.swapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = rendererContent.swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {135.0f / 255.0f, 206.0f / 255.0f, 235.0f / 255.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(rendererContent.commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            
            for(Object* object : Object::objects) {
                // Rendering normal graphics entity instances
                GraphicsEntityInstance<UniformBufferObject>* currentGraphicsEntityInstance = object->getComponent<GraphicsEntityInstance<UniformBufferObject>>();
                Transform* currentTransform = object->getComponent<Transform>(); 

                if(currentGraphicsEntityInstance != nullptr && currentTransform != nullptr) {
                    UniformBufferObject bo = {Camera::mainCamera->view, Camera::mainCamera->projection, currentTransform->getModel()};
                    currentGraphicsEntityInstance->render(rendererContent, bo, i);
                    continue; 
                }
                // Rendering user interface objects
                UIInstance* currentUIInstance = object->getComponent<UIInstance>();

                if(currentUIInstance != nullptr) {
                    UIInstanceUniformBufferObject bo = {currentUIInstance->position, currentUIInstance->size, currentUIInstance->color, currentUIInstance->hover()};
                    currentUIInstance->render(rendererContent, bo, i);
                    continue;
                }

                // TODO: Rendering all custom graphics entities

            }
        
        vkCmdEndRenderPass(rendererContent.commandBuffers[i]);

        if (vkEndCommandBuffer(rendererContent.commandBuffers[i]) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to record command buffer!"); 
            throw std::runtime_error("failed to record command buffer!"); 
        }
    } 

}

uint32_t findMemoryType(RendererContent& rendererContent, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(rendererContent.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
};

void createImage(RendererContent& rendererContent, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {

    Debug::log(INFO, "Creating vulkan image");

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

    if (vkCreateImage(rendererContent.device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        Debug::log(ERROR, "Failed to create image!"); 
        throw std::runtime_error("failed to create image!");
    }
    Debug::log(SUCCESS, "Succesfully created image!"); 

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(rendererContent.device, image, &memRequirements);
    
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(rendererContent, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(rendererContent.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }
    vkBindImageMemory(rendererContent.device, image, imageMemory, 0);

    Debug::log(SUCCESS, "Succesfsfully created vulkan image!"); 
}
VkFormat findSupportedFormat(RendererContent& rendererContent, const std::vector<VkFormat>&candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    
    for (VkFormat format : candidates) {

        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(rendererContent.physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    Debug::log("Failed to find supported format!"); 
    throw std::runtime_error("failed to find supported format!"); 
}
VkFormat findDepthFormat(RendererContent& rendererContent) {
    return findSupportedFormat(rendererContent, {VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}
VkImageView createImageView(RendererContent& rendererContent, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {

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

    if (vkCreateImageView(rendererContent.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}
VkSampleCountFlagBits getMaxUsableSampleCount(RendererContent& rendererContent) {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(rendererContent.physicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}

bool checkDeviceExtensionSupport(RendererContent& rendererContent) {
    
    uint32_t extensionCount; 
    vkEnumerateDeviceExtensionProperties(rendererContent.physicalDevice, nullptr, &extensionCount, nullptr); 
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount); 
    vkEnumerateDeviceExtensionProperties(rendererContent.physicalDevice, nullptr, &extensionCount, availableExtensions.data()); 
    
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end()); 
    
    for (const auto& extension : availableExtensions) { 
        requiredExtensions.erase(extension.extensionName); 
    }

    return requiredExtensions.empty();
}

QueueFamilyIndices findQueueFamilies(RendererContent& rendererContent) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(rendererContent.physicalDevice, &queueFamilyCount,  nullptr); 
 
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount); 
    vkGetPhysicalDeviceQueueFamilyProperties(rendererContent.physicalDevice, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {

        if (indices.isComplete()) {
            break;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { 
            indices.graphicsFamily = i; 
        }

        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(rendererContent.physicalDevice, i, rendererContent.surface, &presentSupport);
         
        if (presentSupport) {
            indices.presentFamily = i;
        }

        i++; 
    }
    return indices; 
}

bool isDeviceSuitable(RendererContent& rendererContent, const VkPhysicalDevice& device) {
    
    Debug::log(INFO, "Checking if device is suitable."); 
    rendererContent.physicalDevice = device;

    QueueFamilyIndices indices = findQueueFamilies(rendererContent); 
    bool extensionsSupported = checkDeviceExtensionSupport(rendererContent); 

    bool swapChainAdequate = false; 

    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport =  querySwapChainSupport(device, rendererContent.surface); 
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

void cleanupSwapChain(RendererContent& rendererContent) {

    for (size_t i = 0; i < rendererContent.swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(rendererContent.device, rendererContent.swapChainFramebuffers[i], nullptr);
    }

    vkFreeCommandBuffers(rendererContent.device, rendererContent.commandPool, static_cast<uint32_t>(rendererContent.commandBuffers.size()), rendererContent.commandBuffers.data());
    

    //TODO: Fix this
    for(Object* object : Object::objects) {
        //vkDestroyPipeline(rendererContent.device, object->renderObject->graphicsPipeline.graphicsPipeline, nullptr);
        //vkDestroyPipelineLayout(rendererContent.device, object->renderObject->graphicsPipeline.pipelineLayout, nullptr);
    }
    vkDestroyRenderPass(rendererContent.device, rendererContent.renderPass, nullptr);

    for (size_t i = 0; i < rendererContent.swapChainImageViews.size(); i++) {
        vkDestroyImageView(rendererContent.device, rendererContent.swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(rendererContent.device, rendererContent.swapChain, nullptr);

};
void reCreateSwapChain(RendererContent& rendererContent, Window& window) {

    if(!window.running)
        return;

    Debug::log(INFO, "Recreating the swapchain!"); 

    int width = 0, height = 0;
    SDL_GetWindowSize(window.sdlWindow, &width, &height);

    while ((width == 0 || height == 0) && window.running) {
        SDL_GetWindowSize(window.sdlWindow, &width, &height);
        SDL_Delay(1); 
    }

    vkDeviceWaitIdle(rendererContent.device);

    cleanupSwapChain(rendererContent); 
    allocateSwapchainDependentRendererContent(rendererContent, window); 

    //TODO: Fix this
    for(Object* object : Object::objects) {
        //object->getComponent<GraphicsEntityInstance>().descriptorPool.create();
    }

    createCommandBuffers(rendererContent, 0);
    // TODO: Re dreate / Allocate command buffers here!

    Debug::log(SUCCESS, "Done recreating swapchain!"); 
}
void drawFrame(RendererContent& rendererContent, Window& window) {

    vkWaitForFences(rendererContent.device, 1, &rendererContent.inFlightFences[rendererContent.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(rendererContent.device, rendererContent.swapChain, UINT64_MAX, rendererContent.imageAvailableSemaphores[rendererContent.currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        reCreateSwapChain(rendererContent, window); 
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    if (rendererContent.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(rendererContent.device, 1, &rendererContent.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    rendererContent.imagesInFlight[imageIndex] = rendererContent.inFlightFences[rendererContent.currentFrame];

   // uniformBuffer.update(imageIndex); 
    createCommandBuffers(rendererContent, imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {rendererContent.imageAvailableSemaphores[rendererContent.currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &rendererContent.commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {rendererContent.renderFinishedSemaphores[rendererContent.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(rendererContent.device, 1, & rendererContent.inFlightFences[rendererContent.currentFrame]);

    if (vkQueueSubmit(rendererContent.graphicsQueue, 1, &submitInfo, rendererContent.inFlightFences[rendererContent.currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {rendererContent.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(rendererContent.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.framebufferResized) {

        window.framebufferResized = false;
        reCreateSwapChain(rendererContent, window); 

    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    //TODO: Change 2 to properties.maxFramesInFlight when yamlparser is done. 
    rendererContent.currentFrame = (rendererContent.currentFrame + 1) % 2;
}
void loop(RendererContent& rendererContent, Window& window, EventManager& eventManager) {

    now = SDL_GetPerformanceCounter(); 
    last = 0; 

    createCommandBuffers(rendererContent, 0); 

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

        drawFrame(rendererContent, window);
        vkDeviceWaitIdle(rendererContent.device);
    }
};
void allocateSwapchainDependentRendererContent(RendererContent& rendererContent, Window& window) {
    {
        Debug::log(INFO, "Setting up swap chain"); 

        SwapChainSupportDetails swapChainSupport =  querySwapChainSupport(rendererContent.physicalDevice, rendererContent.surface); 

        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode =  chooseSwapPresentMode(swapChainSupport.presentModes); 
        VkExtent2D extent =  chooseSwapExtent(swapChainSupport.capabilities, window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount >  swapChainSupport.capabilities.maxImageCount) { 
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = rendererContent.surface;

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(rendererContent);
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

        if (vkCreateSwapchainKHR(rendererContent.device, &createInfo, nullptr, &rendererContent.swapChain)  != VK_SUCCESS) { 
            Debug::log(ERROR, "Failed to create swap chain!"); 
            throw std::runtime_error("failed to create swap chain!");
        }

        vkGetSwapchainImagesKHR(rendererContent.device, rendererContent.swapChain, &imageCount, nullptr);
        rendererContent.swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(rendererContent.device, rendererContent.swapChain, &imageCount, rendererContent.swapChainImages.data());

        rendererContent.swapChainImageFormat = surfaceFormat.format;
        rendererContent.swapChainExtent = extent;

        Debug::log(SUCCESS, "Created swap chain"); 
    }
    {
        Debug::log(INFO, "Create image views");

        rendererContent.swapChainImageViews.resize(rendererContent.swapChainImages.size());

        for (uint32_t i = 0; i < rendererContent.swapChainImages.size(); i++) {
            rendererContent.swapChainImageViews[i] = createImageView(rendererContent, rendererContent.swapChainImages[i], rendererContent.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }

        Debug::log(SUCCESS, "Image views created!"); 
    }
    {

        Debug::log(INFO, "Creating render pass"); 
        
        VkAttachmentDescription depthAttachment{};
        depthAttachment.format = findDepthFormat(rendererContent);
        depthAttachment.samples = rendererContent.msaaSamples;
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
        colorAttachment.format = rendererContent.swapChainImageFormat; 
        colorAttachment.samples = rendererContent.msaaSamples;
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
        colorAttachmentResolve.format = rendererContent.swapChainImageFormat;
        colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

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

        if (vkCreateRenderPass(rendererContent.device, &renderPassInfo, nullptr, &rendererContent.renderPass) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to create render pass!"); 
            throw std::runtime_error("failed to create render pass!");
        }

        Debug::log(SUCCESS, "Successfully created render pass!"); 
    }
    {
        Debug::log(INFO, "Creating color resources!");
        VkFormat colorFormat = rendererContent.swapChainImageFormat;
        createImage(rendererContent, rendererContent.swapChainExtent.width, rendererContent.swapChainExtent.height, 1, rendererContent.msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, rendererContent.colorImage, rendererContent.colorImageMemory);
        rendererContent.colorImageView = createImageView(rendererContent, rendererContent.colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        Debug::log(SUCCESS, "Color resources created!");
    }
    {
        Debug::log(INFO, "Creating depth resources.");
        VkFormat depthFormat = findDepthFormat(rendererContent);
        createImage(rendererContent, rendererContent.swapChainExtent.width, rendererContent.swapChainExtent.height, 1, rendererContent.msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, rendererContent.depthImage, rendererContent.depthImageMemory);
        rendererContent.depthImageView = createImageView(rendererContent, rendererContent.depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);
        Debug::log(SUCCESS, "Created depth resources!"); 
    }
    {

        Debug::log(INFO, "Creating framebuffers"); 

        rendererContent.swapChainFramebuffers.resize(rendererContent.swapChainImageViews.size());

        for (size_t i = 0; i < rendererContent.swapChainImageViews.size(); i++) {
            
            std::array<VkImageView, 3> attachments = {
                rendererContent.colorImageView,
                rendererContent.depthImageView,
                rendererContent.swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{}; 
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = rendererContent.renderPass;
            framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
            framebufferInfo.pAttachments = attachments.data();
            framebufferInfo.width = rendererContent.swapChainExtent.width;
            framebufferInfo.height = rendererContent.swapChainExtent.height;
            framebufferInfo.layers = 1;
            
            if (vkCreateFramebuffer(rendererContent.device, &framebufferInfo, nullptr, &rendererContent.swapChainFramebuffers[i]) != VK_SUCCESS) {
                Debug::log(ERROR, "Failed to create framebuffer!"); 
                throw std::runtime_error("failed to create framebuffer!");
            }
        }
        Debug::log(SUCCESS, "Successfully created all framebuffers.");
    }
}
RendererContent createRenderer(Window& window) {
    RendererContent rendererContent;

    {
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
        SDL_Vulkan_GetInstanceExtensions(window.sdlWindow, &extensionCount, nullptr);

        std::vector<const char *> extensionNames(extensionCount);
        SDL_Vulkan_GetInstanceExtensions(window.sdlWindow, &extensionCount, extensionNames.data());

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

        VkResult result = vkCreateInstance(&createInfo, nullptr, &rendererContent.instance);
        
        if (result != VK_SUCCESS)  { 
            throw std::runtime_error("Failed to create instance!");
            Debug::log(ERROR, "Failed to create instance!"); 
        }

        if (SDL_Vulkan_CreateSurface(window.sdlWindow, rendererContent.instance, &rendererContent.surface) == SDL_FALSE) { 
            Debug::log(ERROR, "Failed to create window surface!"); 
            throw std::runtime_error("Failed to create window surface!"); 
        }

        Debug::log(SUCCESS, "Successfully created a vulkan instance"); 
        Debug::log(SUCCESS, "Successfully created a window surface"); 
    }
    {
        Debug::log(INFO, "Checking and creating physical device context"); 

        uint32_t deviceCount = 0; 
        vkEnumeratePhysicalDevices(rendererContent.instance, &deviceCount, nullptr);

        if (deviceCount == 0) { 
            Debug::log(ERROR, "Failed to find GPUs with Vulkan support!"); 
            throw std::runtime_error("Failed to find GPUs with Vulkan support!"); 
        }

        std::vector<VkPhysicalDevice> devices(deviceCount); 
        vkEnumeratePhysicalDevices(rendererContent.instance, &deviceCount, devices.data());

        for (const auto& device : devices) {
            if (isDeviceSuitable(rendererContent, device)) { 
                rendererContent.physicalDevice = device;
                rendererContent.msaaSamples = getMaxUsableSampleCount(rendererContent);
                break; 
            }
        } 

        if (rendererContent.physicalDevice == VK_NULL_HANDLE) { 
            
            Debug::log(ERROR, "Failed to find a suitable GPU!"); 
            throw std::runtime_error("Failed to find a suitable GPU!");
    
        } else {
        
            vkGetPhysicalDeviceProperties(rendererContent.physicalDevice,&rendererContent.physicalDeviceProperties);
            Debug::log(SUCCESS, "Selected GPU: " + std::string(rendererContent.physicalDeviceProperties.deviceName) + ". Successfully created PhysicalDevice context.");
        }
    }
    {
        Debug::log(INFO, "Setting up a logical device"); 

        QueueFamilyIndices indices = findQueueFamilies(rendererContent);

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

        if (vkCreateDevice(rendererContent.physicalDevice, &createInfo, nullptr, &rendererContent.device) !=  VK_SUCCESS) { 
            
            Debug::log(ERROR, "Failed to create logical device!"); 
            throw std::runtime_error("Failed to create logical device!"); 
        }

        vkGetDeviceQueue(rendererContent.device, indices.graphicsFamily.value(), 0, &rendererContent.graphicsQueue);
        vkGetDeviceQueue(rendererContent.device, indices.presentFamily.value(), 0, &rendererContent.presentQueue);

        Debug::log(SUCCESS, "Created logical device!"); 
    }
    allocateSwapchainDependentRendererContent(rendererContent, window);
    {
        Debug::log(INFO, "Creating command pool."); 

        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(rendererContent);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(); 
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(rendererContent.device, &poolInfo, nullptr, &rendererContent.commandPool) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to create command pool!"); 
            throw std::runtime_error("failed to create command pool!");
        }
        Debug::log(SUCCESS, "Command pool created!"); 
    }
    {

        Debug::log(INFO, "Allocating command buffers");

        rendererContent.commandBuffers.resize(rendererContent.swapChainFramebuffers.size());
        
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = rendererContent.commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) rendererContent.commandBuffers.size();

        if (vkAllocateCommandBuffers(rendererContent.device, &allocInfo, rendererContent.commandBuffers.data()) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to allocate command buffers!"); 
            throw std::runtime_error("failed to allocate command buffers!"); 
        }

        Debug::log(SUCCESS, "Command buffers successfully allocated!");
    }
    {

        Debug::log(INFO, "Creating sync objects"); 

        //TODO: Change 2 with max frames in flight variable from properties. 
        rendererContent.imageAvailableSemaphores.resize(2);
        rendererContent.renderFinishedSemaphores.resize(2);
        rendererContent.imagesInFlight.resize(rendererContent.swapChainImages.size(), VK_NULL_HANDLE);
        rendererContent.inFlightFences.resize(2);


        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        //TODO: Change 2 with max frames in flight variable from properties. 
        for (size_t i = 0; i < 2; i++) {
            
            if (vkCreateSemaphore(rendererContent.device, &semaphoreInfo, nullptr, &rendererContent.imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(rendererContent.device, &semaphoreInfo, nullptr, &rendererContent.renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(rendererContent.device, &fenceInfo, nullptr, &rendererContent.inFlightFences[i]) != VK_SUCCESS) {
                
                Debug::log(ERROR, "Failed to create synchronization objects for a frame!"); 
                throw std::runtime_error("failed to create synchronization objects for a frame!");
            }
        }

        Debug::log(SUCCESS, "Successfully created sync objects!");
    }
    return rendererContent;
}

void destroyRenderer(RendererContent& rendererContent) {

    //TODO: Change 2 with max frames in flight variable from properties.
    for (size_t i = 0; i < 2; i++) {
        vkDestroySemaphore(rendererContent.device, rendererContent.renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(rendererContent.device, rendererContent.imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(rendererContent.device, rendererContent.inFlightFences[i], nullptr);
    }

    for (auto framebuffer : rendererContent.swapChainFramebuffers) {
        vkDestroyFramebuffer(rendererContent.device, framebuffer, nullptr); 
    }

    vkDestroyImageView(rendererContent.device, rendererContent.depthImageView, nullptr);
    vkDestroyImage(rendererContent.device, rendererContent.depthImage, nullptr);
    vkFreeMemory(rendererContent.device, rendererContent.depthImageMemory, nullptr);
    vkDestroyImageView(rendererContent.device, rendererContent.colorImageView, nullptr);
    vkDestroyImage(rendererContent.device, rendererContent.colorImage, nullptr);
    vkFreeMemory(rendererContent.device, rendererContent.colorImageMemory, nullptr);
    vkDestroyCommandPool(rendererContent.device, rendererContent.commandPool, nullptr);
    vkDestroyRenderPass(rendererContent.device, rendererContent.renderPass, nullptr);

    for (auto imageView : rendererContent.swapChainImageViews) {
        vkDestroyImageView(rendererContent.device, imageView, nullptr);
    }

    vkDestroySwapchainKHR(rendererContent.device, rendererContent.swapChain, nullptr);
    vkDestroyDevice(rendererContent.device, nullptr);
    vkDestroySurfaceKHR(rendererContent.instance, rendererContent.surface, nullptr);
    vkDestroyInstance(rendererContent.instance, nullptr);
}


void createBuffer(RendererContent& rendererContent, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(rendererContent.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(rendererContent.device, buffer, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
    findMemoryType(rendererContent, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(rendererContent.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(rendererContent.device, buffer, bufferMemory, 0);
}

VkCommandBuffer beginSingleTimeCommands(RendererContent& rendererContent) {
    
    Debug::log(INFO, "Beginning single-time command");

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = rendererContent.commandPool;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(rendererContent.device, &allocInfo, &commandBuffer);
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    Debug::log(SUCCESS, "Beginning of single-time command is done!"); 
    return commandBuffer;
}
void endSingleTimeCommands(RendererContent& rendererContent, VkCommandBuffer commandBuffer) {

    vkEndCommandBuffer(commandBuffer);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(rendererContent.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(rendererContent.graphicsQueue);

    vkFreeCommandBuffers(rendererContent.device, rendererContent.commandPool, 1, &commandBuffer);
}
bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}
void generateMipmaps(RendererContent& rendererContent, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(rendererContent.physicalDevice, imageFormat, &formatProperties);
    
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(rendererContent);

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
    endSingleTimeCommands(rendererContent, commandBuffer);
}
void copyBuffer(RendererContent& rendererContent, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(rendererContent);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(rendererContent, commandBuffer);
}