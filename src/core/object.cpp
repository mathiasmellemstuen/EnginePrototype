#include "object.h"
#include "../graphics/renderObject.h"
#include <string>
#include <functional>

Object::Object(std::string& name, Transform& transform, std::function<void(float deltaTime)>& update, RenderObject& renderObject) : name(name), transform(transform), update(update), renderObject(renderObject) {

    id += NEXT_ID; 
    NEXT_ID++; 

    objects.push_back(*this); 
};