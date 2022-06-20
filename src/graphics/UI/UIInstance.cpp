#include "UIInstance.h"
#include <glm/vec2.hpp>
#include "../../input/mouse.h"
#include <imgui/imgui.h>

UIInstance::UIInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity) : GraphicsEntityInstance<UIInstanceUniformBufferObject>(rendererContent, graphicsEntity){

}

// This class can be any shape and therfore there is no way to know if you are hovering the item. Returning false. Can be overloaded in the future. 
bool UIInstance::hover() {
    return 0;
}

void UIInstance::debug() {
        //ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
        ImGui::Text("UIInstance"); 

        float arr[2] = {position.x, position.y};
        ImGui::InputFloat2("Position", arr);
        position = {arr[0], arr[1]};

        float sizeArr[2] = {size.x, size.y};
        ImGui::InputFloat2("Size", sizeArr);
        size = {sizeArr[0], sizeArr[1]};

        float colorArr[4] = {color.x, color.y, color.z, color.w};
        ImGui::ColorEdit4("Color", colorArr); 
        color = {colorArr[0], colorArr[1], colorArr[2], colorArr[3]};

        bool checks = hover();
        ImGui::Checkbox("Mouse hovering", &checks);

        int l = layer;
        ImGui::InputInt("Layer", &l);
        layer = l;

}