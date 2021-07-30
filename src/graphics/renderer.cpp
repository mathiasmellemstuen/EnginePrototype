#include "renderer.h"
#include "vulkanInstance.h"
#include "vertexBuffer.h"
#include "vertex.h"
#include "../utility/debug.h"
#include "commandPool.h"

Renderer::Renderer(Window& window, std::vector<Vertex>& verticies, std::vector<uint16_t>& indices):

vulkanInstance(*window.sdlWindow),

physicalDevice(vulkanInstance),

logicalDevice(physicalDevice),

swapChain(physicalDevice, logicalDevice, window),

imageViews(swapChain, logicalDevice),

shader(logicalDevice, "shaders/vert.spv", "shaders/frag.spv"),

descriptorSetLayout(logicalDevice),

graphicsPipeline(physicalDevice, logicalDevice, swapChain, shader, descriptorSetLayout),

commandPool(physicalDevice, logicalDevice), 

depthResources(physicalDevice, logicalDevice, imageViews, swapChain),

frameBuffers(logicalDevice, imageViews, swapChain, graphicsPipeline, depthResources),

texture(physicalDevice, logicalDevice, commandPool, imageViews),

vertexBuffer(physicalDevice, commandPool, logicalDevice, verticies, indices),

uniformBuffer(physicalDevice, logicalDevice, swapChain),

descriptorPool(logicalDevice, swapChain, uniformBuffer, descriptorSetLayout, texture),

commandBuffers(logicalDevice, physicalDevice, frameBuffers, swapChain, graphicsPipeline, vertexBuffer, commandPool, descriptorSetLayout, descriptorPool),

syncObjects(logicalDevice,swapChain),


window(window)

{

};

void Renderer::loop() {
    while(window.running) {
        
        while(SDL_PollEvent(&window.event)) {
            if(window.event.type == SDL_QUIT) {
                window.running = false;
            }
            if (window.event.type == SDL_WINDOWEVENT && window.event.window.event == SDL_WINDOWEVENT_RESIZED && window.event.window.windowID == SDL_GetWindowID(window.sdlWindow)) {

                //Window is resized, re creating swapchain: 
                window.framebufferResized = true; 
            }
        }
        drawFrame(); 
        vkDeviceWaitIdle(logicalDevice.device);

    }
};

void Renderer::cleanupSwapChain() {
    for (size_t i = 0; i < frameBuffers.swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(logicalDevice.device, frameBuffers.swapChainFramebuffers[i], nullptr);
    }

    vkFreeCommandBuffers(logicalDevice.device, commandPool.commandPool, static_cast<uint32_t>(commandBuffers.commandBuffers.size()), commandBuffers.commandBuffers.data());

    vkDestroyPipeline(logicalDevice.device, graphicsPipeline.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice.device, graphicsPipeline.pipelineLayout, nullptr);
    vkDestroyRenderPass(logicalDevice.device, graphicsPipeline.renderPass, nullptr);

    for (size_t i = 0; i < imageViews.swapChainImageViews.size(); i++) {
        vkDestroyImageView(logicalDevice.device, imageViews.swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(logicalDevice.device, swapChain.swapChain, nullptr);
};

void Renderer::reCreateSwapChain() {
    
    int width = 0, height = 0;
    SDL_GetWindowSize(window.sdlWindow, &width, &height);

    while (width == 0 || height == 0) {
        SDL_GetWindowSize(window.sdlWindow, &width, &height);
        SDL_Delay(1); 
    }

    vkDeviceWaitIdle(logicalDevice.device);

    cleanupSwapChain(); 

    swapChain.create(physicalDevice, logicalDevice, window);
    imageViews.create(swapChain, logicalDevice);
    graphicsPipeline.createRenderPass(physicalDevice, swapChain);
    graphicsPipeline.create(logicalDevice, swapChain, shader, descriptorSetLayout);
    depthResources.create(physicalDevice, logicalDevice, imageViews, swapChain); 
    frameBuffers.create(logicalDevice, imageViews, swapChain, graphicsPipeline, depthResources);
    uniformBuffer.create(physicalDevice, logicalDevice, swapChain); 
    descriptorPool.create(logicalDevice, swapChain, uniformBuffer, descriptorSetLayout, texture); 
    commandBuffers.create(logicalDevice, physicalDevice, frameBuffers, swapChain, graphicsPipeline, vertexBuffer, commandPool, descriptorSetLayout, descriptorPool);
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

    uniformBuffer.update(imageIndex); 

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

