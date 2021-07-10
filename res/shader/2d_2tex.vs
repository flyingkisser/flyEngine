#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 texCoord;

//uniform mat4 matModel;
//uniform mat4 matView;
//uniform mat4 matProjection;

void main(){
    //gl_Position = matProjection * matView * matModel * vec4(aPos, 1);
    gl_Position=vec4(aPos,1);
    texCoord=aTexCoord;
}
