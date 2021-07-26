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

CommandBuffers::CommandBuffers(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, VertexBuffer& vertexBuffer, CommandPool& commandPool, DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool) {

    this->device = &logicalDevice.device; 
    create(logicalDevice, physicalDevice, frameBuffers, swapChain, graphicsPipeline, vertexBuffer, commandPool, descriptorSetLayout, descriptorPool);
}
void CommandBuffers::create(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, VertexBuffer& vertexBuffer, CommandPool& commandPool, DescriptorSetLayout& descriptorSetLayout, DescriptorPool& descriptorPool) {

    Debug::log(INFO, "Starting setup and execution of command buffers"); 
    
    commandBuffers.resize(frameBuffers.swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(*device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
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
        renderPassInfo.renderPass = graphicsPipeline.renderPass;
        renderPassInfo.framebuffer = frameBuffers.swapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain.swapChainExtent;

        VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
        renderPassInfo.clearValueCount = 1; 
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.graphicsPipeline);
            VkBuffer vertexBuffers[] = {vertexBuffer.vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffers[i], vertexBuffer.indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &descriptorPool.descriptorSets[i], 0, nullptr);
            vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(vertexBuffer.indices.size()), 1, 0, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to record command buffer!"); 
            throw std::runtime_error("failed to record command buffer!"); 
        }

    }
    Debug::log(SUCCESS, "Successfully executed command buffers"); 
}

VkCommandBuffer CommandBuffers::beginSingleTimeCommands(CommandPool& commandPool) {
    
    Debug::log(INFO, "Beginning single-time command");

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool.commandPool;
    allocInfo.commandBufferCount = 1;
    
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(*device, &allocInfo, &commandBuffer);
    
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);
    
    Debug::log(SUCCESS, "Beginning of single-time command is done!"); 
    return commandBuffer;
}

void CommandBuffers::endSingleTimeCommands(LogicalDevice& logicalDevice, CommandPool& commandPool, VkCommandBuffer commandBuffer) {

    vkEndCommandBuffer(commandBuffer);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;
    vkQueueSubmit(logicalDevice.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicalDevice.graphicsQueue);

    vkFreeCommandBuffers(*device, commandPool.commandPool, 1, &commandBuffer);
}
void CommandBuffers::copyBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {

    VkCommandBuffer commandBuffer = beginSingleTimeCommands(commandPool);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(logicalDevice, commandPool, commandBuffer);
}

CommandBuffers::~CommandBuffers() {

}