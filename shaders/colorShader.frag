#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 1) out vec4 outColor;
layout(binding = 1) uniform vec4 color;
void main() {
  outColor = color;
}