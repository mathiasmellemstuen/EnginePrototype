#include "commandBuffer.h"
#include "logicalDevice.h"
#include "physicalDevice.h"
#include "frameBuffers.h"
#include "graphicsPipeline.h"
#include "../utility/log.h"

#include <stdexcept>
#include <vector>

CommandBuffers::CommandBuffers(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline) {

    this->device = &logicalDevice.device; 
    create(logicalDevice, physicalDevice, frameBuffers, swapChain, graphicsPipeline);
}
void CommandBuffers::create(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline) {

    log(INFO, "Starting setup and execution of command buffers"); 

    QueueFamilyIndices queueFamilyIndices = physicalDevice.findQueueFamilies(physicalDevice.physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(); 
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(*device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        log(ERROR, "Failed to create command pool!"); 
        throw std::runtime_error("failed to create command pool!");
    }

    commandBuffers.resize(frameBuffers.swapChainFramebuffers.size());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    if (vkAllocateCommandBuffers(*device, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        log(ERROR, "Failed to allocate command buffers!"); 
        throw std::runtime_error("failed to allocate command buffers!"); 
    }

    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO; 
        beginInfo.flags = 0; // Optional 
        beginInfo.pInheritanceInfo = nullptr; // Optional 
    
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) { 
            log(ERROR, "Failed to begin recording command buffers!"); 
            throw std::runtime_error("failed to begin recording command buffer!");
        } 

        
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = graphicsPipeline.renderPass;
        renderPassInfo.framebuffer = frameBuffers.swapChainFramebuffers[i];

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChain.swapChainExtent;

        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        renderPassInfo.clearValueCount = 1; 
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.graphicsPipeline);

        vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffers[i]);

        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            log(ERROR, "Failed to record command buffer!"); 
            throw std::runtime_error("failed to record command buffer!"); 
        }
    }
    log(SUCCESS, "Successfully executed command buffers"); 
}
CommandBuffers::~CommandBuffers() {
    vkDestroyCommandPool(*device, commandPool, nullptr);
}