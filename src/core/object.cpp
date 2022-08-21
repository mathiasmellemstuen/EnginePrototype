#include "object.h"
#include <string>
#include <functional>
#include "../utility/debug.h"
#include <imgui/imgui.h>

int Object::NEXT_ID = 0;
std::vector<Object*> Object::objects;

Object::Object(const std::string& name) : name(name) {
    id = NEXT_ID;
    NEXT_ID += 1;
    objects.push_back(this);
};

void Object::debug() {
    std::string title = std::string(Debug::selectedObject->name + " #" + std::to_string(Debug::selectedObject->id)).c_str();


    ImGui::Text("Components: ");

    for(Component* component : components) {
        component->debug();
    }
}
void Object::update(float& deltaTime) {

    for(Component* component : components) {
        component->update(deltaTime);
    }
}

void Object::addComponent(Component* component) {
    components.push_back(component);
}
