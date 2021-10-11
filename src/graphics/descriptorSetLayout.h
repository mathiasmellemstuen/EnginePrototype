#ifndef ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT
#define ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT

#include <vulkan/vulkan.h>

class RenderObject;

class DescriptorSetLayout {

    public:
        VkDescriptorSetLayout descriptorSetLayout;
        void create(); 
        DescriptorSetLayout(RenderObject& renderObject); 
        ~DescriptorSetLayout();
    private:
        RenderObject& renderObject;
};

#endif
