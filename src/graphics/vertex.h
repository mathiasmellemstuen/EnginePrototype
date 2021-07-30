#ifndef ENGINEPROTOTYPE_VERTEX
#define ENGINEPROTOTYPE_VERTEX

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <vulkan/vulkan.h>
#include <array>

struct Vertex {
    glm::vec3 pos; 
    glm::vec3 color;
    glm::vec2 texCoord;

    static VkVertexInputBindingDescription getBindingDescription();
    static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions(); 
};


#endif