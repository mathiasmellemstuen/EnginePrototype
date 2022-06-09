#version 450

layout(binding = 0) uniform UniformBufferObject {
    vec2 position; 
    vec2 size; 
    vec3 color;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec4 outColor;

void main() {
    outUV = inTexCoord; 
    outColor = vec4(ubo.color * inColor, 1.0); 
	gl_Position = vec4(ubo.position, 0.0, 0.0) + (vec4(ubo.size.x, ubo.size.y, 1.0, 1.0) * vec4(inPosition, 1.0));
    gl_Position.y = -gl_Position.y; 
}