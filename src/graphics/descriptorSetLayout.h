#ifndef ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT
#define ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT

#include <vulkan/vulkan.h>

class RendererInfo;

class DescriptorSetLayout {

    public:
        VkDescriptorSetLayout descriptorSetLayout;
        void create(); 
        DescriptorSetLayout(RendererInfo& rendererInfo); 
        ~DescriptorSetLayout();
    private:
        RendererInfo& rendererInfo;
};

#endif