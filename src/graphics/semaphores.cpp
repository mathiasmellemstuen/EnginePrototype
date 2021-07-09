#include "semaphores.h"
#include "logicalDevice.h"
#include <stdexcept>

Semaphores::Semaphores(LogicalDevice& logicalDevice) {

    this->device = &logicalDevice.device;

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &imageAvailableSemaphore) != VK_SUCCESS || vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &renderFinishedSemaphore) != VK_SUCCESS) { 
        throw std::runtime_error("failed to create semaphores!");
    }
};

Semaphores::~Semaphores() {

    vkDestroySemaphore(*device, renderFinishedSemaphore, nullptr);
    vkDestroySemaphore(*device, imageAvailableSemaphore, nullptr);
};

