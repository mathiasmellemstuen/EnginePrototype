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

void drawFrame(LogicalDevice& logicalDevice, SwapChain& swapChain, SyncObjects& syncObjects, CommandBuffers& commandBuffers, GraphicsPipeline& graphicsPipeline) {

    vkWaitForFences(logicalDevice.device, 1, &syncObjects.inFlightFences[syncObjects.currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(logicalDevice.device, 1, &syncObjects.inFlightFences[syncObjects.currentFrame]);
    
    uint32_t imageIndex; 
    vkAcquireNextImageKHR(logicalDevice.device, swapChain.swapChain, UINT64_MAX, syncObjects.imageAvailableSemaphores[syncObjects.currentFrame], VK_NULL_HANDLE, &imageIndex); 

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

    if (vkQueueSubmit(logicalDevice.graphicsQueue, 1, &submitInfo, syncObjects.inFlightFences[syncObjects.currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    // Check if a previous frame is using this image (i.e. there is its fence to wait on)
    if (syncObjects.imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(logicalDevice.device, 1, &syncObjects.imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    syncObjects.imagesInFlight[imageIndex] = syncObjects.inFlightFences[syncObjects.currentFrame]; 

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain.swapChain};
    presentInfo.swapchainCount = 1; 
    presentInfo.pSwapchains = swapChains; 
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional

    vkQueuePresentKHR(logicalDevice.presentQueue, &presentInfo);

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
    SwapChain swapChain(physicalDevice, logicalDevice);
    ImageViews ImageViews(swapChain, logicalDevice);
    Shader shader(logicalDevice, "shaders/vert.spv", "shaders/frag.spv");
    GraphicsPipeline graphicsPipeline(logicalDevice, swapChain, shader);
    FrameBuffers frameBuffers(logicalDevice, ImageViews, swapChain, graphicsPipeline);
    CommandBuffers commandBuffers(logicalDevice, physicalDevice, frameBuffers, swapChain, graphicsPipeline);
    SyncObjects semaphores(logicalDevice, swapChain);

    while(window.running) {
        
        while(SDL_PollEvent(&window.event)) {
            if(window.event.type == SDL_QUIT) {
                window.running = false;
            }
        }
        drawFrame(logicalDevice, swapChain, semaphores, commandBuffers, graphicsPipeline);
        vkDeviceWaitIdle(logicalDevice.device);

    }

    log(INFO, "Exiting application!"); 
    return 0;
}