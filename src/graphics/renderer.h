#ifndef ENGINEPROTOTYPE_RENDERER
#define ENGINEPROTOTYPE_RENDERER

#include "vulkanInstance.h"
#include "window.h"
#include "physicalDevice.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "imageViews.h"
#include "shader.h"
#include "descriptorSetLayout.h"
#include "graphicsPipeline.h"
#include "frameBuffers.h"
#include "commandPool.h"
#include "vertexBuffer.h"
#include "uniformBuffer.h"
#include "descriptorPool.h"
#include "commandBuffer.h"
#include "syncObjects.h"
#include "texture.h"
#include "depthResources.h"

#include <vector>

class Renderer {
    public:
        Window& window; 
        VulkanInstance vulkanInstance;
        PhysicalDevice physicalDevice; 
        LogicalDevice logicalDevice;
        SwapChain swapChain;
        ImageViews imageViews;
        Shader shader;
        DescriptorSetLayout descriptorSetLayout; 
        GraphicsPipeline graphicsPipeline;
        CommandPool commandPool;
        DepthResources depthResources;
        FrameBuffers frameBuffers;
        Texture texture; 
        VertexBuffer vertexBuffer;
        UniformBuffer uniformBuffer; 
        DescriptorPool descriptorPool;
        CommandBuffers commandBuffers;
        SyncObjects syncObjects;

        Renderer(Window& window, std::vector<Vertex>& verticies, std::vector<uint16_t>& indices);
        void loop(); 
    private: 
        void cleanupSwapChain();
        void reCreateSwapChain();
        void drawFrame();

};

#endif