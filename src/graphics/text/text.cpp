#include "text.h"
#include <freetype/freetype.h>
#include <string>
#include <exception>
#include "../../utility/logging.h"
#include "../renderer.h"
#include "../texture.h"

std::map<char, Texture> createCharacterMapFromFont(RendererContent& rendererContent, const std::string& fontPath) {
    logger(INFO, "Creating character map from font"); 

    std::map<char, Texture> map; 

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

    FT_Set_Pixel_Sizes(face, 0, 48);

    //TODO: Fonts with set greater than 128?
    for(unsigned char c = 0; c < 128; c++) {
        
        if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
            logger(ERROR, "Could not find characeter glypth in font"); 
            continue; 
        }

        Texture character; 

        VkDeviceSize size = face->glyph->bitmap.width * face->glyph->bitmap.rows * 4;

        if(size == 0) {
            logger(WARNING, "Size of glyph is zero. Ignoring glyph"); 
            continue; 
        }
        createBuffer(rendererContent, size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, character.stagingBuffer, character.stagingBufferMemory);

        void* data;
        vkMapMemory(rendererContent.device, character.stagingBufferMemory, 0, size, 0, &data);
        mempcpy(data, face->glyph->bitmap.buffer, size);
        vkUnmapMemory(rendererContent.device, character.stagingBufferMemory);
    
        createImage(rendererContent, face->glyph->bitmap.width, face->glyph->bitmap.rows, 1, VK_SAMPLE_COUNT_1_BIT, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, character.textureImage, character.textureImageMemory);

        transitionImageLayout(rendererContent, character.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1);
        copyBufferToImage(rendererContent, character.stagingBuffer, character.textureImage, static_cast<uint32_t>(face->glyph->bitmap.width), static_cast<uint32_t>(face->glyph->bitmap.rows));
        
        vkDestroyBuffer(rendererContent.device, character.stagingBuffer, nullptr); 
        vkFreeMemory(rendererContent.device, character.stagingBufferMemory, nullptr); 

        character.textureImageView = createImageView(rendererContent, character.textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_ASPECT_COLOR_BIT, 1); 
        
        VkPhysicalDeviceProperties properties{};
        vkGetPhysicalDeviceProperties(rendererContent.physicalDevice, &properties);

        VkSamplerCreateInfo samplerInfo{};
        samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
        samplerInfo.magFilter = VK_FILTER_LINEAR;
        samplerInfo.minFilter = VK_FILTER_LINEAR;
        samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
        samplerInfo.anisotropyEnable = VK_TRUE;
        samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
        samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
        samplerInfo.unnormalizedCoordinates = VK_FALSE;
        samplerInfo.compareEnable = VK_FALSE;
        samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
        samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
        samplerInfo.minLod = 0.0f;
        samplerInfo.maxLod = static_cast<float>(0);
        samplerInfo.mipLodBias = 0.0f;

        if (vkCreateSampler(rendererContent.device, &samplerInfo, nullptr, &character.textureSampler) != VK_SUCCESS) {
            logger(ERROR, "Failed to create character image sampler!"); 
            throw std::runtime_error("failed to create character image sampler!");
        }
        map.insert(std::pair<char, Texture>(c, character));
    }
    return map; 
}