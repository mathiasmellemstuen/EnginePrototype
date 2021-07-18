#ifndef ENGINEPROTOTYPE_RENDERER
#define ENGINEPROTOTYPE_RENDERER

#include "vulkanInstance.h"
#include "window.h"
#include "physicalDevice.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "imageViews.h"
#include "shader.h"
#include "graphicsPipeline.h"
#include "frameBuffers.h"
#include "commandBuffer.h"
#include "syncObjects.h"

class Renderer {
    public:
        Window& window; 
        VulkanInstance vulkanInstance;
        PhysicalDevice physicalDevice; 
        LogicalDevice logicalDevice;
        SwapChain swapChain;
        ImageViews imageViews;
        Shader shader;
        GraphicsPipeline graphicsPipeline;
        FrameBuffers frameBuffers;
        CommandBuffers commandBuffers;
        SyncObjects syncObjects;

        Renderer(Window& window);
        void loop(); 
    
    private: 
        void cleanupSwapChain();
        void reCreateSwapChain();
        void drawFrame();

};

#endif