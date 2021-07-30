#include "frameBuffers.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "imageViews.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "graphicsPipeline.h"
#include "../utility/debug.h"
#include "depthResources.h" 
#include <array>

FrameBuffers::FrameBuffers(LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, DepthResources& depthResources) {

    this->device = &logicalDevice.device; 
    create(logicalDevice, imageViews, swapChain, graphicsPipeline, depthResources); 
};
void FrameBuffers::create(LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, DepthResources& depthResources) {

    Debug::log(INFO, "Creating framebuffers"); 

    swapChainFramebuffers.resize(imageViews.swapChainImageViews.size());

    for (size_t i = 0; i < imageViews.swapChainImageViews.size(); i++) {
        
        std::array<VkImageView, 2> attachments = {
            imageViews.swapChainImageViews[i],
            depthResources.depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{}; 
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = graphicsPipeline.renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapChain.swapChainExtent.width;
        framebufferInfo.height = swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;
        
        if (vkCreateFramebuffer(*device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to create framebuffer!"); 
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    Debug::log(SUCCESS, "Successfully created all framebuffers."); 
};

FrameBuffers::~FrameBuffers() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(*device, framebuffer, nullptr); 
    }
};