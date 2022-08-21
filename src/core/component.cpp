#include "component.h"

#include <imgui/imgui.h>

int Component::NEXT_COMPONENT = 0; 

Component::Component() {

    id = NEXT_COMPONENT;
    NEXT_COMPONENT += 1;
    name = "Empty Component";

};
void Component::debug() {
        ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
}
void Component::update(float& deltaTime) {

}
