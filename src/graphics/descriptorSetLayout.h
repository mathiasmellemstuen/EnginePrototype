#ifndef ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT
#define ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT

#include <vulkan/vulkan.h>
#include "logicalDevice.h"

class Renderer;

class DescriptorSetLayout {

    public:
        VkDescriptorSetLayout descriptorSetLayout;
        void create(); 
        DescriptorSetLayout(Renderer& renderer); 
        ~DescriptorSetLayout();
    private:
        Renderer& renderer;
};

#endif