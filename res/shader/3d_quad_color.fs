#version 410 core
precision highp float;

out vec4 FragColor;

uniform vec4 color;

void main(){
    FragColor=vec4(vec3(color),1.0);
}