#include "predefined.h"
#include "../graphics/shader.h"
#include "../graphics/graphicsEntity.h"
#include "../graphics/renderer.h"
#include <map>
#include "../utility/debug.h"
#include "../graphics/UI/UITextEntity.h"

PredefinedShaders predefinedShaders = {}; 
PredefinedGraphicsEntities predefinedGraphicsEntities = {}; 
PredefinedVertexBuffers predefinedVertexBuffers = {};
PredefinedTextEntities predefinedTextEntities = {};

void loadPredefined(RendererContent& rendererContent) {

    // Loading predefined shaders
    predefinedShaders.uiShader = createShader(rendererContent, "assets/shaders/compiled/uiShader.vert.spv", "assets/shaders/compiled/uiShader.frag.spv");
    predefinedShaders.uiCircle = createShader(rendererContent, "assets/shaders/compiled/uiCircle.vert.spv", "assets/shaders/compiled/uiCircle.frag.spv");
    predefinedShaders.uiText = createShader(rendererContent, "assets/shaders/compiled/uiText.vert.spv", "assets/shaders/compiled/uiText.frag.spv");

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
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2, 2, 1, 3};

        predefinedVertexBuffers.rectangle = createVertexBuffer(rendererContent, vertices, indices);
    }
    // Loading predefined graphics entities
    predefinedGraphicsEntities.uiTriangle = createGraphicsEntity(rendererContent, &predefinedShaders.uiShader, &predefinedVertexBuffers.triangle, nullptr, false);
    predefinedGraphicsEntities.uiRectangle = createGraphicsEntity(rendererContent, &predefinedShaders.uiShader, &predefinedVertexBuffers.rectangle, nullptr, false); 
    predefinedGraphicsEntities.uiCircle = createGraphicsEntity(rendererContent, &predefinedShaders.uiCircle, &predefinedVertexBuffers.rectangle, nullptr, false);

    // Loading predefined font entities
    predefinedTextEntities.roboto = createUITextEntity(rendererContent, &predefinedShaders.uiText, &createFont(rendererContent, "assets/fonts/Roboto.ttf", 300));
}