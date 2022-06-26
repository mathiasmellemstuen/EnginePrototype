#ifndef ENGINEPROTOTYPE_TEXT
#define ENGINEPROTOTYPE_TEXT 

#include <freetype/freetype.h>
#include <string>
#include <map>
#include "../texture.h"
#include "../renderer.h"

std::map<char, Texture> createCharacterMapFromFont(RendererContent& rendererContent, const std::string& fontPath);

#endif