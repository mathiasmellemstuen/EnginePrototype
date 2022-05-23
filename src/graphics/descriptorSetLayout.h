#ifndef ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT
#define ENGINEPROTOTYPE_DESCRIPTORSETLAYOUT

#include <vulkan/vulkan.h>
#include <vector>
#include "layoutBinding.h"
class RenderObject;

class DescriptorSetLayout {

    public:
        VkDescriptorSetLayout descriptorSetLayout;
        void create(const std::vector<LayoutBinding>& bindings);
        DescriptorSetLayout(RenderObject& renderObject, const std::vector<LayoutBinding>& bindings);
        void clean();
    private:
        RenderObject& renderObject;
};

#endif
