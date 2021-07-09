#include "graphics/window.h"
#include "graphics/vulkanInstance.h"
#include "graphics/physicalDevice.h"
#include "graphics/logicalDevice.h"
#include "graphics/swapChain.h"
#include "utility/properties.h"
#include "graphics/imageViews.h"
#include "graphics/shader.h"
#include "utility/log.h"
#include "graphics/graphicsPipeline.h"
#include "graphics/frameBuffers.h"
#include "graphics/commandBuffer.h"
#include "graphics/syncObjects.h"
#include "utility/yamlParser.h"

#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

void cleanupSwapChain(LogicalDevice& logicalDevice, FrameBuffers& frameBuffers, CommandBuffers& commandBuffers, GraphicsPipeline& graphicsPipeline, ImageViews& imageViews, SwapChain& swapChain) {
    for (size_t i = 0; i < frameBuffers.swapChainFramebuffers.size(); i++) {
        vkDestroyFramebuffer(logicalDevice.device, frameBuffers.swapChainFramebuffers[i], nullptr);
    }

    vkFreeCommandBuffers(logicalDevice.device, commandBuffers.commandPool, static_cast<uint32_t>(commandBuffers.commandBuffers.size()), commandBuffers.commandBuffers.data());

    vkDestroyPipeline(logicalDevice.device, graphicsPipeline.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(logicalDevice.device, graphicsPipeline.pipelineLayout, nullptr);
    vkDestroyRenderPass(logicalDevice.device, graphicsPipeline.renderPass, nullptr);

    for (size_t i = 0; i < imageViews.swapChainImageViews.size(); i++) {
        vkDestroyImageView(logicalDevice.device, imageViews.swapChainImageViews[i], nullptr);
    }

    vkDestroySwapchainKHR(logicalDevice.device, swapChain.swapChain, nullptr);
}; 

void reCreateSwapChain(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, ImageViews& imageViews, GraphicsPipeline& graphicsPipeline, Shader& shader, FrameBuffers& frameBuffers, CommandBuffers& commandBuffers, SwapChain& swapChain, Window& window) {
    
    int width = 0, height = 0;
    SDL_GetWindowSize(window.sdlWindow, &width, &height);

    while (width == 0 || height == 0) {
        SDL_GetWindowSize(window.sdlWindow, &width, &height);
        SDL_Delay(1); 
    }

    vkDeviceWaitIdle(logicalDevice.device);

    cleanupSwapChain(logicalDevice, frameBuffers, commandBuffers, graphicsPipeline, imageViews, swapChain); 

    swapChain.create(physicalDevice, logicalDevice, window);
    imageViews.create(swapChain, logicalDevice);
    graphicsPipeline.createRenderPass(swapChain);
    graphicsPipeline.create(logicalDevice, swapChain, shader);
    frameBuffers.create(logicalDevice, imageViews, swapChain, graphicsPipeline);
    commandBuffers.create(logicalDevice, physicalDevice, frameBuffers, swapChain, graphicsPipeline);
};

void drawFrame(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain, SyncObjects& syncObjects, CommandBuffers& commandBuffers, GraphicsPipeline& graphicsPipeline, ImageViews& imageViews, Shader& shader, FrameBuffers& frameBuffers, Window& window) {

    // vkWaitForFences(logicalDevice.device, 1, &syncObjects.inFlightFences[syncObjects.currentFrame], VK_TRUE, UINT64_MAX);
    // vkResetFences(logicalDevice.device, 1, &syncObjects.inFlightFences[syncObjects.currentFrame]);
    
    // uint32_t imageIndex; 
    // vkAcquireNextImageKHR(logicalDevice.device, swapChain.swapChain, UINT64_MAX, syncObjects.imageAvailableSemaphores[syncObjects.currentFrame], VK_NULL_HANDLE, &imageIndex); 

    // VkSubmitInfo submitInfo{};
    // submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO; 

    // VkSemaphore waitSemaphores[] = {syncObjects.imageAvailableSemaphores[syncObjects.currentFrame]}; 
    // VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    // submitInfo.waitSemaphoreCount = 1; 
    // submitInfo.pWaitSemaphores = waitSemaphores; 
    // submitInfo.pWaitDstStageMask = waitStages;
    // submitInfo.commandBufferCount = 1;
    // submitInfo.pCommandBuffers = &commandBuffers.commandBuffers[imageIndex];

    // VkSemaphore signalSemaphores[] = {syncObjects.renderFinishedSemaphores[syncObjects.currentFrame]};
    // submitInfo.signalSemaphoreCount = 1; 
    // submitInfo.pSignalSemaphores = signalSemaphores;

    // if (vkQueueSubmit(logicalDevice.graphicsQueue, 1, &submitInfo, syncObjects.inFlightFences[syncObjects.currentFrame]) != VK_SUCCESS) {
    //     throw std::runtime_error("failed to submit draw command buffer!");
    // }

    // // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    // if (syncObjects.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
    //     vkWaitForFences(logicalDevice.device, 1, &syncObjects.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    // }
    // // Mark the image as now being in use by this frame
    // syncObjects.imagesInFlight[imageIndex] = syncObjects.inFlightFences[syncObjects.currentFrame]; 

    // VkPresentInfoKHR presentInfo{};
    // presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    // presentInfo.waitSemaphoreCount = 1;
    // presentInfo.pWaitSemaphores = signalSemaphores;

    // VkSwapchainKHR swapChains[] = {swapChain.swapChain};
    // presentInfo.swapchainCount = 1; 
    // presentInfo.pSwapchains = swapChains; 
    // presentInfo.pImageIndices = &imageIndex;
    // presentInfo.pResults = nullptr; // Optional

    // vkQueuePresentKHR(logicalDevice.presentQueue, &presentInfo);

    // syncObjects.currentFrame = (syncObjects.currentFrame + 1) % properties.maxFramesInFlight;
        vkWaitForFences(logicalDevice.device, 1, &syncObjects.inFlightFences[syncObjects.currentFrame], VK_TRUE, UINT64_MAX);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(logicalDevice.device, swapChain.swapChain, UINT64_MAX, syncObjects.imageAvailableSemaphores[syncObjects.currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            reCreateSwapChain(physicalDevice, logicalDevice, imageViews, graphicsPipeline, shader, frameBuffers, commandBuffers, swapChain, window);
            return;
        } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        if (syncObjects.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(logicalDevice.device, 1, &syncObjects.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        syncObjects.imagesInFlight[imageIndex] = syncObjects.inFlightFences[syncObjects.currentFrame];

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

        result = vkQueuePresentKHR(logicalDevice.presentQueue, &presentInfo);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.framebufferResized) {
            window.framebufferResized = false;
            reCreateSwapChain(physicalDevice, logicalDevice, imageViews, graphicsPipeline, shader, frameBuffers, commandBuffers, swapChain, window);
        } else if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to present swap chain image!");
        }

        syncObjects.currentFrame = (syncObjects.currentFrame + 1) % properties.maxFramesInFlight;
};

int main(int argc, char *argv[]) {
    log(INFO, "Starting application."); 

    loadPropertiesFromFile();
    printProperties();

    runTest();
    printResult();

    Window window;
    VulkanInstance vulkanInstance(*(window.sdlWindow));
    PhysicalDevice physicalDevice(vulkanInstance);
    LogicalDevice logicalDevice(physicalDevice); 
    SwapChain swapChain(physicalDevice, logicalDevice, window);
    ImageViews imageViews(swapChain, logicalDevice);
    Shader shader(logicalDevice, "shaders/vert.spv", "shaders/frag.spv");
    GraphicsPipeline graphicsPipeline(logicalDevice, swapChain, shader);
    FrameBuffers frameBuffers(logicalDevice, imageViews, swapChain, graphicsPipeline);
    CommandBuffers commandBuffers(logicalDevice, physicalDevice, frameBuffers, swapChain, graphicsPipeline);
    SyncObjects semaphores(logicalDevice, swapChain);

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
        drawFrame(physicalDevice, logicalDevice, swapChain, semaphores, commandBuffers, graphicsPipeline, imageViews, shader, frameBuffers, window);
        vkDeviceWaitIdle(logicalDevice.device);

    }

    log(INFO, "Exiting application!"); 
    return 0;
}

