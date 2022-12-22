#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;
layout (location=2) in vec3 aNormal;

out vec3 localPos;

uniform mat4 proj;
uniform mat4 view;

void main(){
    localPos=aPos;
    gl_Position = proj * view * vec4(aPos, 1);
}