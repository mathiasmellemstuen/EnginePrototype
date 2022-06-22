#version 450

layout (location = 0) in vec2 inUV;
layout (location = 1) in vec4 inColor;
layout (location = 2) in float inHover; 
layout (location = 3) in vec2 inPosition; 
layout (location = 4) in vec2 inSize; 

layout (location = 0) out vec4 outColor;

void main() {

    vec2 currentPixelCoord = gl_FragCoord.xy;
    currentPixelCoord.x = currentPixelCoord.x / 1920; 
    currentPixelCoord.y = currentPixelCoord.y / 1080;
    currentPixelCoord.x = (currentPixelCoord.x - 0.5) / 0.5;
    currentPixelCoord.y = (currentPixelCoord.y - 0.5) / 0.5;

    vec2 middle = inPosition;
    
    float e1 = (abs(currentPixelCoord.x) - middle.x) / inSize.x; 
    float e2 = (abs(currentPixelCoord.y) - middle.y) / inSize.y; 
    float d = (e1 * e1) + (e2 * e2); 

    if(d > 1) {
        discard;
    }

    outColor = inColor;  
    outColor.w = outColor.w * smoothstep(1.01, 0.99, sqrt(d));

    if(inHover > 0.5) {
        outColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}