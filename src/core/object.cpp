#include "object.h"
#include "../graphics/renderObject.h"
#include <string>
#include <functional>
#include "../utility/debug.h"
#include <imgui/imgui.h>
#include "../graphics/renderInstance.h"

int Object::NEXT_ID = 0;
std::vector<Object*> Object::objects;

Object::Object(const std::string& name, RenderObject* renderObject) : name(name), renderObject(renderObject), renderInstance(renderObject) {

    id = NEXT_ID;
    NEXT_ID += 1;


    objects.push_back(this);

    update = [&](float& deltaTime){
    };

    debug = [&](){
        std::string title = std::string(Debug::selectedObject->name + " #" + std::to_string(Debug::selectedObject->id)).c_str();

        float  arr[3] = {renderInstance.transform.position.x, renderInstance.transform.position.y, renderInstance.transform.position.z};
        ImGui::InputFloat3("Position", arr);
        renderInstance.transform.position = {arr[0], arr[1], arr[2]};

        ImGui::SliderFloat("Angle", &renderInstance.transform.angle, -360, 360);

        float arr2[3] = {renderInstance.transform.rotationDirection.x, renderInstance.transform.rotationDirection.y, renderInstance.transform.rotationDirection.z};
        ImGui::InputFloat3("Rotation direction", arr2);
        renderInstance.transform.rotationDirection = {arr2[0], arr2[1], arr2[2]};

        ImGui::Text("Components: ");

        for(Component* component : components) {

            component->debug();
        }
    };
};

void Object::addComponent(Component* component) {
    components.push_back(component);
}