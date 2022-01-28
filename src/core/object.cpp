#include "object.h"
#include "../graphics/renderObject.h"
#include <string>
#include <functional>
#include "../utility/debug.h"

int Object::NEXT_ID = 0; 
std::vector<Object> Object::objects;

Object::Object(std::string& name, Transform& transform, RenderObject& renderObject) : name(name), transform(transform), renderObject(renderObject) {
    update = [](float deltaTime){};
    id += NEXT_ID; 
    NEXT_ID++; 

    objects.push_back(*this); 
};

void Object::setUpdate(std::function<void(float deltaTime)>& update) {
    this->update = update;
}