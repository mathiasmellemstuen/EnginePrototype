#version 450

layout(binding = 0) uniform UIInstanceUniformBufferObject {
    vec2 position;
    vec2 size;
    vec3 color;
    bool hover;
} ubo;


layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

out gl_PerVertex {
	vec4 gl_Position;   
};

layout (location = 0) out vec2 outUV;
layout (location = 1) out vec4 outColor;

void main() {
    outUV = inTexCoord; 
    outColor = vec4(inColor, 1.0); 
    gl_Position = vec4(ubo.position.x + inPosition.x, ubo.position.y + inPosition.y, 0.0, 1.0);
}