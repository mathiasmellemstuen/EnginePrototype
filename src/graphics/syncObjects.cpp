#include "syncObjects.h"
#include "logicalDevice.h"
#include <stdexcept>
#include "../utility/debug.h"
#include "swapChain.h"

SyncObjects::SyncObjects(LogicalDevice& logicalDevice, SwapChain& swapChain) {

    Debug::log(INFO, "Creating sync objects"); 

    this->device = &logicalDevice.device;

    //TODO: Change 2 with max frames in flight variable from properties. 
    imageAvailableSemaphores.resize(2);
    renderFinishedSemaphores.resize(2);
    imagesInFlight.resize(swapChain.swapChainImages.size(), VK_NULL_HANDLE);
    inFlightFences.resize(2);


    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    //TODO: Change 2 with max frames in flight variable from properties. 
    for (size_t i = 0; i < 2; i++) {
        
        if (vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS || vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS || vkCreateFence(*device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            
            Debug::log(ERROR, "Failed to create synchronization objects for a frame!"); 
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }

    Debug::log(SUCCESS, "Successfully created sync objects!");
};

SyncObjects::~SyncObjects() {

    //TODO: Change 2 with max frames in flight variable from properties.
    for (size_t i = 0; i < 2; i++) {
        vkDestroySemaphore(*device, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(*device, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(*device, inFlightFences[i], nullptr);
    }
};

