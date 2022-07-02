#include "font.h"
#include <freetype/freetype.h>
#include <string>
#include <exception>
#include <stdexcept>
#include "../utility/logging.h"
#include "renderer.h"
#include "texture.h"
#include "vertex.h"
#include <map>

Font& createFont(RendererContent& rendererContent, const std::string& fontPath, int pixelSize) {
    logger(INFO, "Creating character map from font"); 

    Font* font = new Font;
    font->pixelSize = pixelSize; 

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

    int maxUnderBaseLine = 0; 
    uint32_t imageWidth = (pixelSize + 2) * 16; 
    uint32_t imageHeight = (pixelSize + 2) * 8; 
    VkDeviceSize deviceSize = imageWidth * imageHeight; 
    unsigned char* buffer = new unsigned char[imageWidth * imageHeight];
    memset(buffer, 0, imageWidth * imageHeight);

    for(uint16_t c = 0; c < 128; ++c) {

        unsigned int glyphIndex = FT_Get_Char_Index(face, c);
        FT_Load_Glyph(face, glyphIndex, FT_LOAD_DEFAULT);
        FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);

        int glyphHang = (face->glyph->metrics.horiBearingY - face->glyph->metrics.height) / 64;
        if(glyphHang < maxUnderBaseLine) {
            maxUnderBaseLine = glyphHang; 
        }

        int x = (c % 16) * (pixelSize + 2);
        int y = (c / 16) * (pixelSize + 2); 


        y += (pixelSize + 2) - face->glyph->bitmap_top + maxUnderBaseLine - 1; 

        const FT_Bitmap& bitmap = face->glyph->bitmap;

        for(int xx = 0; xx < bitmap.width; ++xx) {
            for(int yy = 0; yy < bitmap.rows; ++yy) {
                unsigned char value = bitmap.buffer[(yy * (bitmap.width) + xx)];
                buffer[(y + yy) * imageWidth + (x + xx)] = value;
            }
        }
        std::vector<Vertex> vertices = {
            {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {(float)x / (float)imageWidth, (float)y / (float)imageHeight}},
            {{-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {(float)x / (float)imageWidth, ((float)y + (float)bitmap.rows) / (float)imageHeight}},
            {{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {((float)x + (float)bitmap.width) / (float)imageWidth, (float)y / (float)imageHeight}},
            {{1.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {((float)x + (float)bitmap.width) / (float)imageWidth, ((float)y + (float)bitmap.rows) / (float)imageHeight}},
        };

        std::vector<uint32_t> indices = {0, 1, 2, 2, 1, 3};
        font->allCharacterVertexBuffers.insert(std::pair<char, VertexBuffer>(c, createVertexBuffer(rendererContent, vertices, indices)));

        GlyphMetrics metrics; 
        metrics.width = static_cast<int>(face->glyph->metrics.width / 64); 
        metrics.height = static_cast<int>(face->glyph->metrics.height / 64); 
        metrics.advance = static_cast<int>(face->glyph->advance.x / 64); 
        metrics.bearingX = static_cast<int>(face->glyph->metrics.horiBearingX / 64); 
        metrics.bearingY = static_cast<int>(face->glyph->metrics.horiBearingY / 64); 

        metrics.left = face->glyph->bitmap_left; 
        metrics.top = face->glyph->bitmap_top; 

        font->allCharacterMetrics.insert(std::pair<char, GlyphMetrics>(c, metrics));
    }

    createBuffer(rendererContent, deviceSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, font->texture.stagingBuffer, font->texture.stagingBufferMemory);
    void* data;
    vkMapMemory(rendererContent.device, font->texture.stagingBufferMemory, 0, deviceSize, 0, &data);
    memcpy(data, buffer, deviceSize);
    vkUnmapMemory(rendererContent.device, font->texture.stagingBufferMemory);

    uint32_t mipLevels = 1; 
    createImage(rendererContent, imageWidth, imageHeight, mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8_UNORM, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, font->texture.textureImage, font->texture.textureImageMemory);

    transitionImageLayout(rendererContent, font->texture.textureImage, VK_FORMAT_R8_UNORM, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, mipLevels);
    copyBufferToImage(rendererContent, font->texture.stagingBuffer, font->texture.textureImage, imageWidth, imageHeight);

    vkDestroyBuffer(rendererContent.device, font->texture.stagingBuffer, nullptr); 
    vkFreeMemory(rendererContent.device, font->texture.stagingBufferMemory, nullptr); 

    font->texture.textureImageView = createImageView(rendererContent, font->texture.textureImage, VK_FORMAT_R8_UNORM, VK_IMAGE_ASPECT_COLOR_BIT, mipLevels); 
    
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(rendererContent.physicalDevice, &properties);

    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
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

    if (vkCreateSampler(rendererContent.device, &samplerInfo, nullptr, &font->texture.textureSampler) != VK_SUCCESS) {
        logger(ERROR, "Failed to create character image sampler!"); 
        throw std::runtime_error("failed to create character image sampler!");
    }
    return *font; 
}