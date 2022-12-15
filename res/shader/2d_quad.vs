#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 view_pos;

layout (std140) uniform mat2d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;

void main(){
    gl_Position=vec4(aPos,1);
    texCoord=aTexCoord;
    view_pos=cam_pos;
}