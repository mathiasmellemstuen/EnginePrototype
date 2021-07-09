#include "syncObjects.h"
#include "logicalDevice.h"
#include <stdexcept>
#include "../utility/properties.h"
#include "../utility/log.h"
#include "swapChain.h"

SyncObjects::SyncObjects(LogicalDevice& logicalDevice, SwapChain& swapChain) {

    this->device = &logicalDevice.device;

    imageAvailableSemaphores.resize(properties.maxFramesInFlight);
    renderFinishedSemaphores.resize(properties.maxFramesInFlight);
    imagesInFlight.resize(swapChain.swapChainImages.size(), VK_NULL_HANDLE);
    inFlightFences.resize(properties.maxFramesInFlight);


    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < properties.maxFramesInFlight; i++) {
        
        if (vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(*device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            
            log(ERROR, "Failed to create synchronization objects for a frame!"); 
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
};

SyncObjects::~SyncObjects() {

    for (size_t i = 0; i < properties.maxFramesInFlight; i++) {
        vkDestroySemaphore(*device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(*device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(*device, inFlightFences[i], nullptr);
    }
};

