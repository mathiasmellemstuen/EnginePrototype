#include "object.h"
#include "../graphics/renderObject.h"
#include <string>
#include <functional>
#include "../utility/debug.h"

int Object::NEXT_ID = 0; 
std::vector<Object*> Object::objects;

Object::Object(const std::string& name, RenderObject* renderObject) : renderObject(renderObject) {

    this->name = name; 
    id = NEXT_ID;
    NEXT_ID += 1;

    objects.push_back(this); 
};

void Object::addComponent(Component* component) {
    components.push_back(component);
};