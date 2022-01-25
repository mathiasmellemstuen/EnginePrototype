#include "renderer.h"
#include "vulkanInstance.h"
#include "vertexBuffer.h"
#include "vertex.h"
#include "../utility/debug.h"
#include "commandPool.h"
#include <string>
#include "../input/mouseInput.h"

Renderer::Renderer(Window& window):

window(window),

vulkanInstance(*this),

physicalDevice(*this),

logicalDevice(*this),

swapChain(*this),

imageViews(*this),

renderPass(*this),

commandPool(*this), 

colorResources(*this),

depthResources(*this),

frameBuffers(*this),

uniformBuffer(*this),

commandBuffers(*this),

syncObjects(*this)

{};

void Renderer::loop() {
    
    commandBuffers.create(0); 
    while(window.running) {

        Debug::calculateFps(); 
        
        if(!Debug::debugWindowRunning) {
            window.running = false;
        }

        eventManager.update(*window.sdlWindow);

        drawFrame(); 
        vkDeviceWaitIdle(logicalDevice.device);
        Debug::drawDebugWindow(eventManager.event);
    }
};

void Renderer::cleanupSwapChain() {

    for (size_t i = 0; i < frameBuffers.swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(logicalDevice.device, frameBuffers.swapChainFramebuffers[i], nullptr);
    }

    vkFreeCommandBuffers(logicalDevice.device, commandPool.commandPool, static_cast<uint32_t>(commandBuffers.commandBuffers.size()), commandBuffers.commandBuffers.data());

    //TODO: Find a way to destroy the current pipeline...
    //vkDestroyPipeline(logicalDevice.device, graphicsPipeline.graphicsPipeline, nullptr);
    //vkDestroyPipelineLayout(logicalDevice.device, I>graphicsPipeline.pipelineLayout, nullptr);
    vkDestroyRenderPass(logicalDevice.device, renderPass.renderPass, nullptr);

    for (size_t i = 0; i < imageViews.swapChainImageViews.size(); i++) {
        vkDestroyImageView(logicalDevice.device, imageViews.swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(logicalDevice.device, swapChain.swapChain, nullptr);
};

void Renderer::reCreateSwapChain() {
    
    Debug::log(INFO, "Recreating the swapchain!"); 

    int width = 0, height = 0;
    SDL_GetWindowSize(window.sdlWindow, &width, &height);

    while (width == 0 || height == 0) {
        SDL_GetWindowSize(window.sdlWindow, &width, &height);
        SDL_Delay(1); 
    }

    vkDeviceWaitIdle(logicalDevice.device);

    cleanupSwapChain(); 

    swapChain.create();
    imageViews.create();
    renderPass.create();
    colorResources.create(); 
    depthResources.create(); 
    frameBuffers.create();
    uniformBuffer.create(); 
        //TODO: Find a way to create the current descriptor pool
    //rendererInfo->descriptorPool.create(); 
    currentRenderObject->descriptorPool.create(); 
    //commandBuffers.create(0);
    commandBuffers.allocateCommandBuffers();
};

void Renderer::drawFrame() {

    vkWaitForFences(logicalDevice.device, 1, &syncObjects.inFlightFences[syncObjects.currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(logicalDevice.device, swapChain.swapChain, UINT64_MAX, syncObjects.imageAvailableSemaphores[syncObjects.currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        reCreateSwapChain(); 
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    if (syncObjects.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(logicalDevice.device, 1, &syncObjects.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    syncObjects.imagesInFlight[imageIndex] = syncObjects.inFlightFences[syncObjects.currentFrame];

    //uniformBuffer.update(imageIndex); 
    commandBuffers.create(imageIndex);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {syncObjects.imageAvailableSemaphores[syncObjects.currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers.commandBuffers[imageIndex];

    VkSemaphore signalSemaphores[] = {syncObjects.renderFinishedSemaphores[syncObjects.currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    vkResetFences(logicalDevice.device, 1, & syncObjects.inFlightFences[syncObjects.currentFrame]);

    if (vkQueueSubmit(logicalDevice.graphicsQueue, 1, &submitInfo, syncObjects.inFlightFences[syncObjects.currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain.swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(this->logicalDevice.presentQueue, &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.framebufferResized) {

        window.framebufferResized = false;
        reCreateSwapChain(); 

    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    //TODO: Change 2 to properties.maxFramesInFlight when yamlparser is done. 
    syncObjects.currentFrame = (syncObjects.currentFrame + 1) % 2;
};

void Renderer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    
    if (vkCreateBuffer(logicalDevice.device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(logicalDevice.device, buffer, &memRequirements);
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
    findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(logicalDevice.device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(logicalDevice.device, buffer, bufferMemory, 0);
}

uint32_t Renderer::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
};

VkCommandBuffer Renderer::beginSingleTimeCommands() {
    
    Debug::log(INFO, "Beginning single-time command");

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool.commandPool;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(logicalDevice.device, &allocInfo, &commandBuffer);
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    Debug::log(SUCCESS, "Beginning of single-time command is done!"); 
    return commandBuffer;
}

void Renderer::endSingleTimeCommands(VkCommandBuffer commandBuffer) {

    vkEndCommandBuffer(commandBuffer);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(logicalDevice.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicalDevice.graphicsQueue);

    vkFreeCommandBuffers(logicalDevice.device, commandPool.commandPool, 1, &commandBuffer);
}

VkFormat Renderer::findSupportedFormat(const std::vector<VkFormat>&candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    
    for (VkFormat format : candidates) {

        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(physicalDevice.physicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        } else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    Debug::log("Failed to find supported format!"); 
    throw std::runtime_error("failed to find supported format!"); 
}

VkFormat Renderer::findDepthFormat() {
    return findSupportedFormat({VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT}, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
}

bool Renderer::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

void Renderer::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory) {

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

    if (vkCreateImage(logicalDevice.device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        Debug::log(ERROR, "Failed to create image!"); 
        throw std::runtime_error("failed to create image!");
    }
    Debug::log(SUCCESS, "Succesfully created image!"); 

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(logicalDevice.device, image, &memRequirements);
    
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(logicalDevice.device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }
    vkBindImageMemory(logicalDevice.device, image, imageMemory, 0);

    Debug::log(SUCCESS, "Succesfsfully created vulkan image!"); 
}

void Renderer::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {

    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(physicalDevice.physicalDevice, imageFormat, &formatProperties);
    
    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        throw std::runtime_error("texture image format does not support linear blitting!");
    }

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

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
    endSingleTimeCommands(commandBuffer);
}

VkSampleCountFlagBits Renderer::getMaxUsableSampleCount() {
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice.physicalDevice, &physicalDeviceProperties);

    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;

    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}

void Renderer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

void Renderer::useRenderObject(RenderObject& renderObject) {
    currentRenderObject = &renderObject;
}
