#ifndef ENGINEPROTOTYPE_SEMAPHORES
#define ENGINEPROTOTYPE_SEMAPHORES

#include <vulkan/vulkan.h>
#include "logicalDevice.h"

class Semaphores {
    public:
        VkSemaphore imageAvailableSemaphore;
        VkSemaphore renderFinishedSemaphore;
        Semaphores(LogicalDevice& logicalDevice); 
        ~Semaphores(); 
    
    private:
        VkDevice* device; 
};

#endif