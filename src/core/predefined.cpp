#include "predefined.h"
#include "../graphics/shader.h"
#include "../graphics/graphicsEntity.h"
#include "../graphics/renderer.h"
#include <map>
#include "../utility/debug.h"
#include "../graphics/UI/UITextEntity.h"
#include "../utility/logging.h"

PredefinedShaders predefinedShaders = {}; 
PredefinedGraphicsEntities predefinedGraphicsEntities = {}; 
PredefinedVertexBuffers predefinedVertexBuffers = {};
PredefinedTextEntities predefinedTextEntities = {};

void loadPredefined(Renderer& renderer) {

    // Loading predefined shaders
    predefinedShaders.uiShader = createShader(renderer, "assets/shaders/compiled/uiShader.vert.spv", "assets/shaders/compiled/uiShader.frag.spv");
    predefinedShaders.uiCircle = createShader(renderer, "assets/shaders/compiled/uiCircle.vert.spv", "assets/shaders/compiled/uiCircle.frag.spv");
    predefinedShaders.uiText = createShader(renderer, "assets/shaders/compiled/uiText.vert.spv", "assets/shaders/compiled/uiText.frag.spv");

    // Creating vertex buffers 
    {
        std::vector<Vertex> vertices = {
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}
        };

        std::vector<uint32_t> indices = {0, 1, 2};

        predefinedVertexBuffers.triangle = createVertexBuffer(renderer, vertices, indices);
    }
    {
        std::vector<Vertex> vertices = {
            {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
            {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}},
        };

        std::vector<uint32_t> indices = {0, 1, 2, 2, 1, 3};

        predefinedVertexBuffers.rectangle = createVertexBuffer(renderer, vertices, indices);
    }
    logger(INFO, "Before exception!");
    // Loading predefined graphics entities
    predefinedGraphicsEntities.uiTriangle = createGraphicsEntity(renderer, renderer.renderPasses.at("UI"), &predefinedShaders.uiShader, &predefinedVertexBuffers.triangle, nullptr, false);
    logger(INFO, "After exception");
    predefinedGraphicsEntities.uiRectangle = createGraphicsEntity(renderer, renderer.renderPasses.at("UI"), &predefinedShaders.uiShader, &predefinedVertexBuffers.rectangle, nullptr, false);
    predefinedGraphicsEntities.uiCircle = createGraphicsEntity(renderer, renderer.renderPasses.at("UI"), &predefinedShaders.uiCircle, &predefinedVertexBuffers.rectangle, nullptr, false);

    // Loading predefined font entities
    predefinedTextEntities.roboto = createUITextEntity(renderer, &predefinedShaders.uiText, &createFont(renderer, "assets/fonts/Roboto.ttf", 150));
}