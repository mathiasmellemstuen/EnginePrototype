#version 450

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec4 inColor;
layout (location = 2) in float inHover; 
layout (location = 3) in vec2 inTexCoord;

layout(binding = 1) uniform sampler2D texSampler; 

layout (location = 0) out vec4 outColor;

void main() {
  outColor = vec4(inColor.x, inColor.y, inColor.z, inColor.w * texture(texSampler, inTexCoord).r);

  if(inHover > 0.5) {
    outColor = vec4(1.0, 1.0, 1.0, texture(texSampler, inTexCoord).r);
  }
}