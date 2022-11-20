#version 330 core

layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 texCoord;
out vec2 texCoord_gs;

layout (std140) uniform mat2d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;

void main(){
    gl_Position=proj*matModel*vec4(aPos,0,1);
    // gl_Position=vec4(aPos,1);
    texCoord=aTexCoord;
    texCoord_gs=aTexCoord;
}
