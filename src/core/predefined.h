#ifndef ENGINEPROTOTYPE_PREDEFINED
#define ENGINEPROTOTYPE_PREDEFINED

#include "../graphics/graphicsEntity.h"
#include "../graphics/shader.h"
#include "../graphics/renderer.h"
#include "../graphics/vertexBuffer.h"
#include "../graphics/text/text.h"
#include "../graphics/texture.h"

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
    GraphicsEntity uiText;
};

struct PredefinedVertexBuffers {
    VertexBuffer triangle;
    VertexBuffer rectangle;
};

struct PredefinedFonts {
    std::map<char, Texture> roboto;
};

extern PredefinedShaders predefinedShaders; 
extern PredefinedGraphicsEntities predefinedGraphicsEntities; 
extern PredefinedVertexBuffers predefinedVertexBuffers;
extern PredefinedFonts predefinedFonts;

void loadPredefined(RendererContent& rendereContent);

#endif