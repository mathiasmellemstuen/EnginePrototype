#ifndef ENGINEPROTOTYPE_SEMAPHORES
#define ENGINEPROTOTYPE_SEMAPHORES

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include <vector>
#include "swapChain.h"

class SyncObjects {
    public:
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> imagesInFlight;
        std::vector<VkFence> inFlightFences;
        size_t currentFrame = 0;

        SyncObjects(LogicalDevice& logicalDevice, SwapChain& swapChain); 
        ~SyncObjects(); 
    
    private:
        VkDevice* device; 
};

#endif