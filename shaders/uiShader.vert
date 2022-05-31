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

layout(location = 0) out vec3 fragColor;

void main() {
    vec2 pos[3] = vec2[3]( vec2(-0.7, 0.7),
                           vec2(0.7, 0.7),
                           vec2(0.0, -0.7) );
    gl_Position = vec4( pos[gl_VertexIndex], 0.0, 1.0 );
    fragColor = inColor;
}