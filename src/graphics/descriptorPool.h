#ifndef ENGINEPROTOTYPE_DESCRIPTORPOOL
#define ENGINEPROTOTYPE_DESCRIPTORPOOL

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "swapChain.h"
#include <vector>
#include "uniformBuffer.h"
#include "descriptorSetLayout.h"
#include "texture.h"

class Renderer;

class DescriptorPool {
    public:
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        void create();
        void createDescriptorSets();
        DescriptorPool(Renderer& renderer); 
        ~DescriptorPool(); 
    private:
        Renderer& renderer;
};

#endif