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

#include "renderer.h"

CommandBuffers::CommandBuffers(Renderer& renderer) : renderer(renderer) {

    create();
}
void CommandBuffers::create() {

    Debug::log(INFO, "Starting setup and execution of command buffers");
    
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

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.graphicsPipeline.graphicsPipeline);
            VkBuffer vertexBuffers[] = {renderer.vertexBuffer.vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffers[i], renderer.vertexBuffer.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, renderer.graphicsPipeline.pipelineLayout, 0, 1, &renderer.descriptorPool.descriptorSets[i], 0, nullptr);
            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(renderer.vertexBuffer.indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to record command buffer!"); 
            throw std::runtime_error("failed to record command buffer!"); 
        }

    }
    Debug::log(SUCCESS, "Successfully executed command buffers"); 
}

CommandBuffers::~CommandBuffers() {

}