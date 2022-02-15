#ifndef ENGINEPROTOTYPE_UNIFORMBUFFER
#define ENGINEPROTOTYPE_UNIFORMBUFFER

#include <vector>
#include <vulkan/vulkan.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class RenderInstance;

class UniformBuffer {
    public:
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        void update(uint32_t currentImage, glm::mat4& view, glm::mat4& projection, glm::mat4& model);
        void create();
        UniformBuffer(RenderInstance& renderInstance);
        void clean(); 
    private:
        size_t allocatedSwapChainSize;
        RenderInstance& renderInstance;
};

#endif
