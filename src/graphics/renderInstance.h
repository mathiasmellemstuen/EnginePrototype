//
// Created by Mathias on 15.02.2022.
//

#ifndef ENGINEPROTOTYPE_RENDERINSTANCE_H
#define ENGINEPROTOTYPE_RENDERINSTANCE_H

#include "../core/transform.h"

class RenderObject;
#include "uniformBuffer.h"
#include "descriptorPool.h"

class RenderInstance {
    public:
        RenderObject* renderObject;
        UniformBuffer uniformBuffer;
        DescriptorPool descriptorPool;
        Transform transform;
        RenderInstance(RenderObject* renderObject);
};
#endif //ENGINEPROTOTYPE_RENDERINSTANCE_H
