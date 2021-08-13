#ifndef ENGINEPROTOTYPE_DESCRIPTORPOOL
#define ENGINEPROTOTYPE_DESCRIPTORPOOL

#include <vulkan/vulkan.h>
#include <vector>

class RendererInfo;

class DescriptorPool {
    public:
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        void create();
        void createDescriptorSets();
        DescriptorPool(RendererInfo& rendererInfo); 
        ~DescriptorPool(); 
    private:
        RendererInfo& rendererInfo;
};

#endif