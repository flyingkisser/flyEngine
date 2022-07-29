#version 330 core
layout(location=0) in vec3 aPos;

out vec3 texCoords;

uniform mat4 matProj;
uniform mat4 matModel;
uniform mat4 matCamera;

void main(){
    vec4 pos=matProj*matCamera*vec4(aPos,1.0);
    gl_Position = pos.xyww;
    texCoords=aPos;
}
