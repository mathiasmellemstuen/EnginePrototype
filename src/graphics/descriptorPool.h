#ifndef ENGINEPROTOTYPE_DESCRIPTORPOOL
#define ENGINEPROTOTYPE_DESCRIPTORPOOL

#include <vulkan/vulkan.h>
#include <vector>

class RenderObject;

class DescriptorPool {
    public:
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        void create();
        void createDescriptorSets();
        DescriptorPool(RenderObject& renderObject); 
        void clean(); 
    private:
        RenderObject& renderObject;
};

#endif
