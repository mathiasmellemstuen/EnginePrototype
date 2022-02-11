#include "commandBuffer.h"
#include "logicalDevice.h"
#include "physicalDevice.h"
#include "frameBuffers.h"
#include "graphicsPipeline.h"
#include "../utility/debug.h"
#include "commandPool.h"
#include "vertexBuffer.h"
#include "descriptorPool.h"

#include <stdexcept>
#include <vector>
#include <array>
#include <functional>

#include "renderer.h"
#include "vulkan/vulkan_core.h"
#include "renderObject.h"
#include "../core/object.h"

#include "camera.h"

CommandBuffers::CommandBuffers(Renderer& renderer) : renderer(renderer) {
    
    allocateCommandBuffers();
}

void CommandBuffers::allocateCommandBuffers() {
    
    Debug::log(INFO, "Allocating command buffers");

    commandBuffers.resize(renderer.frameBuffers.swapChainFramebuffers.size());
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = renderer.commandPool.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(renderer.logicalDevice.device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        Debug::log(ERROR, "Failed to allocate command buffers!"); 
        throw std::runtime_error("failed to allocate command buffers!"); 
    }

    Debug::log(SUCCESS, "Command buffers successfully allocated!");
}
void CommandBuffers::create(uint32_t currentImage) {
    
    for (size_t i = 0; i < commandBuffers.size(); i++) {
        
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    

        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) { 
            Debug::log(ERROR, "Failed to begin recording command buffers!"); 
            throw std::runtime_error("failed to begin recording command buffer!");
        } 
        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderer.renderPass.renderPass;
        renderPassInfo.framebuffer = renderer.frameBuffers.swapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = renderer.swapChain.swapChainExtent;

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.0f, 0.0f, 0.0f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0}; 
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            
            for(Object* object : Object::objects) {
                if(object->renderObject != nullptr) {
                    object->renderObject->render(*Camera::mainCamera, object->transform, commandBuffers[i], i, currentImage);
                }
            }
        
        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to record command buffer!"); 
            throw std::runtime_error("failed to record command buffer!"); 
        }
    } 

};

void CommandBuffers::clean() {

};