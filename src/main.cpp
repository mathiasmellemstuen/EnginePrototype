#include "graphics/window.h"
#include "graphics/vulkanInstance.h"
#include "graphics/physicalDevice.h"
#include "graphics/logicalDevice.h"
#include "graphics/swapChain.h"
#include "utility/properties.h"
#include "graphics/imageViews.h"
#include "graphics/shader.h"
#include "utility/log.h"
#include "graphics/graphicsPipeline.h"
#include "graphics/frameBuffers.h"

#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    log(INFO, "Starting application."); 

    loadPropertiesFromFile();
    printProperties();

    Window window;
    VulkanInstance vulkanInstance(*(window.sdlWindow));
    PhysicalDevice physicalDevice(vulkanInstance);
    LogicalDevice logicalDevice(physicalDevice); 
    SwapChain swapChain(physicalDevice, logicalDevice);
    ImageViews ImageViews(swapChain, logicalDevice);
    Shader shader(logicalDevice, "shaders/vert.spv", "shaders/frag.spv");
    GraphicsPipeline graphicsPipeline(logicalDevice, swapChain, shader);
    FrameBuffers frameBuffers(logicalDevice, ImageViews, swapChain, graphicsPipeline);

    while(window.running) {
        
        while(SDL_PollEvent(&window.event)) {
            if(window.event.type == SDL_QUIT) {
                window.running = false;
            }
        }
    }

    log(INFO, "Exiting application!"); 
    return 0;
}