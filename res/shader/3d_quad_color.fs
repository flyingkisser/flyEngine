#version 410 core
precision highp float;

in vec2 texCoord;
out vec4 FragColor;

uniform vec3 color;

void main(){
    FragColor=vec4(color,1.0);
}