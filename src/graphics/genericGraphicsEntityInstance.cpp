#include "genericGraphicsEntityInstance.h"
#include <imgui/imgui.h>
#include "renderer.h"
#include <map>

GenericGraphicsEntityInstance::GenericGraphicsEntityInstance(Renderer& renderer, const std::string& renderPassName) {
	this->renderPassObject = getRenderPassObject(renderer, renderPassName.c_str());
	this->renderPassObject->renderInstances.push_back(this);
}
void setLayer(uint16_t newLayer) {
	// TODO: Sort the vector in renderPassObject. 
}
void GenericGraphicsEntityInstance::debug() {
    ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
}
void GenericGraphicsEntityInstance::reCreateGraphics(Renderer& renderer) {

}
void GenericGraphicsEntityInstance::render(Renderer& renderer, int currentCommandBufferIndex) {

}
