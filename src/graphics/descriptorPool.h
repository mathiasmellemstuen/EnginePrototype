#ifndef ENGINEPROTOTYPE_DESCRIPTORPOOL
#define ENGINEPROTOTYPE_DESCRIPTORPOOL

#include <vulkan/vulkan.h>
#include <vector>

class RenderInstance;

class DescriptorPool {
    public:
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        void create();
        void createDescriptorSets();
        DescriptorPool(RenderInstance& renderInstance);
        void clean(); 
    private:
        RenderInstance& renderInstance;
};

#endif
