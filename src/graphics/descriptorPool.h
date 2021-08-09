#ifndef ENGINEPROTOTYPE_DESCRIPTORPOOL
#define ENGINEPROTOTYPE_DESCRIPTORPOOL

#include <vulkan/vulkan.h>
#include <vector>

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