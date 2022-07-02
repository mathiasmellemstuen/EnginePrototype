#ifndef ENGINEPROTOTYPE_FONT
#define ENGINEPROTOTYPE_FONT

#include <freetype/freetype.h>
#include <string>
#include <map>
#include "texture.h"
#include "renderer.h"
#include <glm/glm.hpp>
#include "vertexBuffer.h"

struct GlyphMetrics {
    int width; 
    int height; 
    int bearingX; 
    int bearingY; 
    int advanceX; 
    int advanceY; 
};

struct Font {
    std::map<char, VertexBuffer> allCharacterVertexBuffers;
    std::map<char, GlyphMetrics> allCharacterMetrics; 
    Texture texture;
    int pixelSize; 
};

Font& createFont(RendererContent& rendererContent, const std::string& fontPath, int pixelSize = 150);

#endif