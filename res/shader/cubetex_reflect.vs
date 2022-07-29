#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;

uniform mat4 matProj;
uniform mat4 matCamera;
uniform mat4 matModel;

out vec3 vectorNormal;
out vec3 posFrag;
out vec3 originPos;
out vec2 texCoord;

void main(){
    vectorNormal=mat3(transpose(inverse(matModel))) * aNormal;
    posFrag=vec3(matModel*vec4(aPos,1));
    texCoord=aTexCoord;
    gl_Position=matProj*matCamera*matModel*vec4(aPos,1);
}
