#ifndef ENGINEPROTOTYPE_SEMAPHORES
#define ENGINEPROTOTYPE_SEMAPHORES

#include <vulkan/vulkan.h>
#include <vector>

class Renderer;

class SyncObjects {
    public:
        std::vector<VkSemaphore> imageAvailableSemaphores;
        std::vector<VkSemaphore> renderFinishedSemaphores;
        std::vector<VkFence> imagesInFlight;
        std::vector<VkFence> inFlightFences;
        size_t currentFrame = 0;

        SyncObjects(Renderer& renderer); 
        ~SyncObjects(); 
    
    private:
        Renderer& renderer; 
};

#endif