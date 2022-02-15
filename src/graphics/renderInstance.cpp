#include "renderInstance.h"
#include "renderObject.h"
#include "uniformBuffer.h"
#include "descriptorPool.h"
#include "../utility/debug.h"

RenderInstance::RenderInstance(RenderObject *renderObject) : renderObject(renderObject), uniformBuffer(*this), descriptorPool(*this){

    // Not creating this instance for objects that does not require any rendering
    if(renderObject == nullptr) {
        return;
    }

    uniformBuffer.create();
    descriptorPool.create();
}