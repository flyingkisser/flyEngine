#version 330 core
layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 matProj;
uniform mat4 matModel;

void main(){
    gl_Position=matProj*matModel*vec4(aPos,0,1);
//    gl_Position=vec4(aPos,0,1);
//    gl_Position=matModel*vec4(aPos,0,1);
    texCoord=aTexCoord;
}
