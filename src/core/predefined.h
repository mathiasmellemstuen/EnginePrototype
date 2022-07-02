#ifndef ENGINEPROTOTYPE_PREDEFINED
#define ENGINEPROTOTYPE_PREDEFINED

#include "../graphics/graphicsEntity.h"
#include "../graphics/shader.h"
#include "../graphics/renderer.h"
#include "../graphics/vertexBuffer.h"
#include "../graphics/font.h"
#include "../graphics/texture.h"
#include "../graphics/UI/UITextEntity.h"
#include <map>

struct PredefinedShaders {
    Shader uiShader; 
    Shader uiCircle;
    Shader uiText;
};

struct PredefinedGraphicsEntities {
    GraphicsEntity uiTriangle;
    GraphicsEntity uiRectangle;
    GraphicsEntity uiCircle; 
};

struct PredefinedVertexBuffers {
    VertexBuffer triangle;
    VertexBuffer rectangle;
};

struct PredefinedTextEntities {
    UITextEntity roboto;
};

extern PredefinedShaders predefinedShaders; 
extern PredefinedGraphicsEntities predefinedGraphicsEntities; 
extern PredefinedVertexBuffers predefinedVertexBuffers;
extern PredefinedTextEntities predefinedTextEntities;

void loadPredefined(RendererContent& rendereContent);

#endif