#version 330 core

in vec2 UV;
out vec3 color;

uniform sampler2D myTextureSampler;

void main(){
    color = texture(myTextureSampler, vec2((UV.x + 1.3125) * 0.0625, 1 - (UV.y * 0.0625))).rgb;
}