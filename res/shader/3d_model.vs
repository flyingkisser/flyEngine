#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;


out vec2 texCoord;
out vec3 normalVector;
out vec3 posFrag;

uniform mat4 matModel;
uniform mat4 matCamera;
uniform mat4 matProj;

void main(){
    gl_Position = matProj * matCamera * matModel * vec4(aPos, 1);
    posFrag=vec3(matModel * vec4(aPos, 1));
    texCoord=aTexCoord;
    normalVector = mat3(transpose(inverse(matModel))) * aNormal;
}
