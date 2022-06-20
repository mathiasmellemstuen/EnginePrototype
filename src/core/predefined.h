#ifndef ENGINEPROTOTYPE_PREDEFINED
#define ENGINEPROTOTYPE_PREDEFINED

#include "../graphics/graphicsEntity.h"
#include "../graphics/shader.h"
#include "../graphics/renderer.h"
#include "../graphics/vertexBuffer.h"

struct PredefinedShaders {
    Shader uiShader; 
};

struct PredefinedGraphicsEntities {
    GraphicsEntity uiTriangle;
    GraphicsEntity uiRectangle;
};

struct PredefinedVertexBuffers {
    VertexBuffer triangle;
    VertexBuffer rectangle;
};

extern PredefinedShaders predefinedShaders; 
extern PredefinedGraphicsEntities predefinedGraphicsEntities; 
extern PredefinedVertexBuffers predefinedVertexBuffers;

void loadPredefined(RendererContent& rendereContent);

#endif