#ifndef ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT
#define ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT

#include <vulkan/vulkan.h>
#include "logicalDevice.h"

class DescriptorSetLayout {

    public:
        VkDevice* device; 
        VkDescriptorSetLayout descriptorSetLayout;
        void create(LogicalDevice& logicalDevice); 
        DescriptorSetLayout(LogicalDevice& logicalDevice); 
        ~DescriptorSetLayout();
};

#endif