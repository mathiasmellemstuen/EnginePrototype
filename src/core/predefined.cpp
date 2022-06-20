#include "predefined.h"
#include "../graphics/shader.h"
#include "../graphics/graphicsEntity.h"
#include "../graphics/renderer.h"

PredefinedShaders predefinedShaders = {}; 
PredefinedGraphicsEntities predefinedGraphicsEntities = {}; 
PredefinedVertexBuffers predefinedVertexBuffers = {};

void loadPredefined(RendererContent& rendererContent) {

    // Loading predefined shaders
    predefinedShaders.uiShader = createShader(rendererContent, "assets/shaders/compiled/uiShader.vert.spv", "assets/shaders/compiled/uiShader.frag.spv");
    

    // Creating vertex buffers 
    {
        std::vector<Vertex> vertices = {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        predefinedVertexBuffers.triangle = createVertexBuffer(rendererContent, vertices, indices);
    }
    {
        std::vector<Vertex> vertices = {
            {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2, 2, 1, 3};

        predefinedVertexBuffers.rectangle = createVertexBuffer(rendererContent, vertices, indices);
    }
    // Loading predefined graphics entities
    predefinedGraphicsEntities.uiTriangle = createGraphicsEntity(rendererContent, &predefinedShaders.uiShader, &predefinedVertexBuffers.triangle, nullptr, false);
    predefinedGraphicsEntities.uiRectangle = createGraphicsEntity(rendererContent, &predefinedShaders.uiShader, &predefinedVertexBuffers.rectangle, nullptr, false); 
}