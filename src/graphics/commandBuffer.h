#ifndef ENGINEPROTOTYPE_COMMANDBUFFER
#define ENGINEPROTOTYPE_COMMANDBUFFER

#include <vulkan/vulkan.h>
#include <vector>

class Renderer;

class CommandBuffers {
    public:
        std::vector<VkCommandBuffer> commandBuffers;
        CommandBuffers(Renderer& renderer); 
        void create(uint32_t currentImage);
        ~CommandBuffers(); 
    private:
        Renderer& renderer;
};

#endif