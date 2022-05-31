#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) out vec4 outColor;
layout(location = 0) in vec3 fragColor;
//layout(binding = 1) uniform vec4 color;

void main() {
  outColor = vec4(1.0, 1.0, 1.0, 1.0);
}