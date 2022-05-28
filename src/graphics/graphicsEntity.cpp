#include "graphicsEntity.h"
#include "renderer.h"
#include <vulkan/vulkan.h>
#include "../utility/debug.h"

GraphicsEntity createGraphicsEntity(RendererContent& rendererContent, VertexBuffer* vertexBuffer, Texture* texture, Shader* shader, const std::vector<LayoutBinding>& bindings) {
    GraphicsEntity graphicsEntity; 
    graphicsEntity.vertexBuffer = vertexBuffer;
    graphicsEntity.texture = texture;
    graphicsEntity.shader = shader; 

    Debug::log(INFO, "Starting setup of Descriptor set layout");

    std::vector<VkDescriptorSetLayoutBinding> lBindings;

    int bindingIndex = 0;

    for(const LayoutBinding& binding : bindings) {
        VkDescriptorSetLayoutBinding b;
        b.binding = bindingIndex;
        b.descriptorCount = 1;
        b.descriptorType = binding.type;
        b.stageFlags = binding.stage;
        b.pImmutableSamplers = nullptr;
        bindingIndex += 1;
        lBindings.push_back(b);
    }

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(lBindings.size());
    layoutInfo.pBindings = lBindings.data();

    if (vkCreateDescriptorSetLayout(rendererContent.device, &layoutInfo, nullptr, &graphicsEntity.descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    Debug::log(SUCCESS, "Descriptor set layout setup successfull!"); 
    Debug::log(INFO, "Starting to create graphics pipeline"); 

    auto bindingDescription = Vertex::getBindingDescription();
    auto attributeDescriptions = Vertex::getAttributeDescriptions(); 

    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType =  VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO; 
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType =  VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO; 
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; 
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport{};
    viewport.x = 0.0f; 
    viewport.y = 0.0f; 
    viewport.width = (float) rendererContent.swapChainExtent.width; 
    viewport.height = (float) rendererContent.swapChainExtent.height; 
    viewport.minDepth = 0.0f; 
    viewport.maxDepth = 1.0f;

    VkRect2D scissor{};
    scissor.offset = {0, 0}; 
    scissor.extent = rendererContent.swapChainExtent;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType =  VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;

    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizer{}; 
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO; 
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;

    rasterizer.lineWidth = 1.0f;


    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;

    rasterizer.depthBiasEnable = VK_FALSE;
    rasterizer.depthBiasConstantFactor = 0.0f; // Optional
    rasterizer.depthBiasClamp = 0.0f; // Optional
    rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO; 
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = rendererContent.msaaSamples;
    multisampling.minSampleShading = 1.0f; // Optional 
    multisampling.pSampleMask = nullptr; // Optional 
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional 
    multisampling.alphaToOneEnable = VK_FALSE; // Optional¨

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional 
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional 
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional 
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional 
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    colorBlendAttachment.blendEnable = VK_TRUE;
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA; 
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO; 
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1; 
    colorBlending.pAttachments = &colorBlendAttachment; 
    colorBlending.blendConstants[0] = 0.0f; // Optional 
    colorBlending.blendConstants[1] = 0.0f; // Optional 
    colorBlending.blendConstants[2] = 0.0f; // Optional 
    colorBlending.blendConstants[3] = 0.0f; // Optional


    VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO; 
    dynamicState.dynamicStateCount = 2;
    dynamicState.pDynamicStates = dynamicStates;


    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &graphicsEntity.descriptorSetLayout;

    if (vkCreatePipelineLayout(rendererContent.device, &pipelineLayoutInfo, nullptr, &graphicsEntity.pipelineLayout) != VK_SUCCESS) { 
        Debug::log(ERROR, "Failed to create pipeline layout!"); 
        throw std::runtime_error("failed to create pipeline layout!"); 
    }

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE;
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.minDepthBounds = 0.0f; // Optional
    depthStencil.maxDepthBounds = 1.0f; // Optional
    depthStencil.stencilTestEnable = VK_FALSE;


    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shader->shaderStages;
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.layout = graphicsEntity.pipelineLayout;
    pipelineInfo.renderPass = rendererContent.renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineInfo.pDepthStencilState = &depthStencil;

    if (vkCreateGraphicsPipelines(rendererContent.device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsEntity.graphicsPipeline) != VK_SUCCESS) {
        
        Debug::log(ERROR, "Failed to create graphics pipeline!"); 
        throw std::runtime_error("failed to create graphics pipeline!"); 
    }
    Debug::log(SUCCESS, "Created graphics pipeline!"); 
    return graphicsEntity; 
}

void freeGraphicsEntity(RendererContent& rendererContent, GraphicsEntity& graphicsEntity) {
    vkDestroyPipeline(rendererContent.device, graphicsEntity.graphicsPipeline, nullptr);
    vkDestroyPipelineLayout(rendererContent.device, graphicsEntity.pipelineLayout, nullptr);
    vkDestroyDescriptorSetLayout(rendererContent.device, graphicsEntity.descriptorSetLayout, nullptr);
}
