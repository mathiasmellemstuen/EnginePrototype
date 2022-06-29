#ifndef ENGINEPROTOTYPE_TEXT
#define ENGINEPROTOTYPE_TEXT 

#include <freetype/freetype.h>
#include <string>
#include <map>
#include "../texture.h"
#include "../renderer.h"
#include <glm/glm.hpp>


struct TextureCoordinateSet {
    glm::vec2 top; 
    glm::vec2 left; 
    glm::vec2 right; 
    glm::vec2 bottom;
};
struct GlyphAtlas {
    std::map<uint16_t, TextureCoordinateSet> glypthToTextureCoordinateSet;
    Texture texture;
};


const GlyphAtlas& createGlyphAtlasFromFont(RendererContent& rendererContent, const std::string& fontPath, int pixelSize = 48);

#endif