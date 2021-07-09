#include "frameBuffers.h"
#include <vulkan/vulkan.h>
#include <vector>
#include "imageViews.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "graphicsPipeline.h"
#include "../utility/log.h"

FrameBuffers::FrameBuffers(LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline) {

    this->device = &logicalDevice.device; 
    create(logicalDevice, imageViews, swapChain, graphicsPipeline); 
};
void FrameBuffers::create(LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline) {

    log(INFO, "Creating framebuffers"); 

    swapChainFramebuffers.resize(imageViews.swapChainImageViews.size());

    for (size_t i = 0; i < imageViews.swapChainImageViews.size(); i++) {
        
        VkImageView attachments[] = {
            imageViews.swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{}; 
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = graphicsPipeline.renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = swapChain.swapChainExtent.width;
        framebufferInfo.height = swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;
        
        if (vkCreateFramebuffer(*device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            log(ERROR, "Failed to create framebuffer!"); 
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    log(SUCCESS, "Successfully created all framebuffers."); 
};

FrameBuffers::~FrameBuffers() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(*device, framebuffer, nullptr); 
    }
};

