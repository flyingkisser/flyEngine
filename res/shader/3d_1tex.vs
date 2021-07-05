#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 matModel;
uniform mat4 matCamera;
uniform mat4 matProj;

void main(){
    gl_Position = matProj * matCamera * matModel * vec4(aPos, 1);
    texCoord=aTexCoord;
}
