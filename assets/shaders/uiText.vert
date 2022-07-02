#version 450

layout( push_constant ) uniform UniformBufferObject {
    vec2 position; 
    vec2 size; 
    vec4 color;
    float hover;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec4 outColor;
layout (location = 2) out float outHover;
layout (location = 3) out vec2 outTexCoord;

void main() {
    outUV = inTexCoord; 
    outColor = ubo.color * vec4(inColor, 1.0); 
    outHover = ubo.hover;
    outTexCoord = inTexCoord;
    gl_Position = vec4(ubo.position, 0, 0) + vec4(inPosition.x * ubo.size.x, inPosition.y * ubo.size.y, inPosition.z, 1.0); 
    gl_Position.y = -gl_Position.y; 
}