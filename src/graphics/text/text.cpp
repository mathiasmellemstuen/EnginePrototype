#include "text.h"
#include <freetype/freetype.h>
#include <string>
#include <exception>
#include <stdexcept>
#include "../../utility/logging.h"
#include "../renderer.h"
#include "../texture.h"
#include <map>

VkDeviceSize calculateDeviceSize(FT_Face& face, uint16_t maxCharacter) {
    VkDeviceSize size; 

    for(uint16_t c = 0; c < maxCharacter; c++) {

        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            logger(ERROR, "Could not find characeter glypth in font"); 
            continue; 
        }
        size += face->glyph->bitmap.width * face->glyph->bitmap.rows;
    }
    size *= 4; // Because of 4 bytes per?

    return size;
}
const GlyphAtlas& createGlyphAtlasFromFont(RendererContent& rendererContent, const std::string& fontPath, int pixelSize) {
    logger(INFO, "Creating character map from font"); 

    GlyphAtlas* glyphAtlas = new GlyphAtlas;

    FT_Library ft; 

    if(FT_Init_FreeType(&ft)) {

        logger(ERROR, "Could not initiate freetype library");
        throw std::runtime_error("Could not initiate freetype library");
    }

    FT_Face face; 

    if(FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
        logger(ERROR, "Could not load font");
        throw std::runtime_error("could not load font"); 
    }

    FT_Set_Pixel_Sizes(face, 0, pixelSize);
    
    std::vector<unsigned char> finalBuffer; 
    uint16_t lastEndIndex = -1; 

    for(uint16_t c = 15; c < 16; c++) {
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            logger(ERROR, "Could not find characeter glypth in font"); 
            continue; 
        }

        unsigned int width = face->glyph->bitmap.width; 
        unsigned int height = face->glyph->bitmap.rows; 

        uint16_t glyphLength = face->glyph->bitmap.width * face->glyph->bitmap.rows;

        for(uint16_t i = 0; i < glyphLength; i++) {
            finalBuffer.push_back(face->glyph->bitmap.buffer[i]);
        }

        uint16_t end = lastEndIndex + glyphLength; 

        TextureCoordinateSet textureCoordinateSet = {
            {0.0f, 1.0f},
            {1.0f, 0.0f},
            {1.0f, 1.0f},
            {0.0f, 0.0f}
        };
        glyphAtlas->glypthToTextureCoordinateSet.insert(std::pair<uint16_t, TextureCoordinateSet>(c, textureCoordinateSet));

        lastEndIndex = end;
        logger(INFO, "Finished loop!"); 
    }

    VkDeviceSize deviceSize = finalBuffer.size() * 4;
    createBuffer(rendererContent, deviceSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, glyphAtlas->texture.stagingBuffer, glyphAtlas->texture.stagingBufferMemory);
    void* data;
    vkMapMemory(rendererContent.device, glyphAtlas->texture.stagingBufferMemory, 0, deviceSize, 0, &data);
    memcpy(data, finalBuffer.data(), deviceSize);
    vkUnmapMemory(rendererContent.device, glyphAtlas->texture.stagingBufferMemory);

    // The width and height? 
    uint32_t imageWidth = face->glyph->bitmap.width; 
    uint32_t imageHeight = face->glyph->bitmap.rows;

    //uint32_t mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(imageWidth, imageHeight)))) + 1;
    uint32_t mipLevels = 1; 
    createImage(rendererContent, imageWidth, imageHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, glyphAtlas->texture.textureImage, glyphAtlas->texture.textureImageMemory);

    transitionImageLayout(rendererContent, glyphAtlas->texture.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
    copyBufferToImage(rendererContent, glyphAtlas->texture.stagingBuffer, glyphAtlas->texture.textureImage, imageWidth, imageHeight);

    vkDestroyBuffer(rendererContent.device, glyphAtlas->texture.stagingBuffer, nullptr); 
    vkFreeMemory(rendererContent.device, glyphAtlas->texture.stagingBufferMemory, nullptr); 

    glyphAtlas->texture.textureImageView = createImageView(rendererContent, glyphAtlas->texture.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels); 
    
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(rendererContent.physicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = static_cast<float>(mipLevels);
    samplerInfo.mipLodBias = 0.0f;

    if (vkCreateSampler(rendererContent.device, &samplerInfo, nullptr, &glyphAtlas->texture.textureSampler) != VK_SUCCESS) {
        logger(ERROR, "Failed to create character image sampler!"); 
        throw std::runtime_error("failed to create character image sampler!");
    }
    //std::map.insert(std::pair<char, Texture>(c, character));
    return *glyphAtlas; 
}