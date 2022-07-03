#include "genericGraphicsEntityInstance.h"
#include <imgui/imgui.h>

void GenericGraphicsEntityInstance::debug() {
    ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
}
void GenericGraphicsEntityInstance::reCreateGraphics(Renderer& renderer) {

}
void GenericGraphicsEntityInstance::render(Renderer& renderer, int currentCommandBufferIndex) {

}