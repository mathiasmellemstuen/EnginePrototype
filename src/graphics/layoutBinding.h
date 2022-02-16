//
// Created by Mathias on 15.02.2022.
//

#ifndef ENGINEPROTOTYPE_LAYOUTBINDING_H
#define ENGINEPROTOTYPE_LAYOUTBINDING_H

#include <vulkan/vulkan.h>

struct LayoutBinding {
    VkDescriptorType type;
    VkShaderStageFlags stage;
    int bufferRange;
};
#endif //ENGINEPROTOTYPE_LAYOUTBINDING_H
