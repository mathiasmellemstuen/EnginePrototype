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
#include "colorResources.h"
#include "renderer.h"

FrameBuffers::FrameBuffers(Renderer& renderer) : renderer(renderer) {
    create(); 
};
void FrameBuffers::create() {

    Debug::log(INFO, "Creating framebuffers"); 

    swapChainFramebuffers.resize(renderer.imageViews.swapChainImageViews.size());

    for (size_t i = 0; i < renderer.imageViews.swapChainImageViews.size(); i++) {
        
        std::array<VkImageView, 3> attachments = {
            renderer.colorResources.colorImageView,
            renderer.depthResources.depthImageView,
            renderer.imageViews.swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{}; 
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderer.graphicsPipeline.renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = renderer.swapChain.swapChainExtent.width;
        framebufferInfo.height = renderer.swapChain.swapChainExtent.height;
        framebufferInfo.layers = 1;
        
        if (vkCreateFramebuffer(renderer.logicalDevice.device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            Debug::log(ERROR, "Failed to create framebuffer!"); 
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
    Debug::log(SUCCESS, "Successfully created all framebuffers."); 
};

FrameBuffers::~FrameBuffers() {
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(renderer.logicalDevice.device, framebuffer, nullptr); 
    }
};