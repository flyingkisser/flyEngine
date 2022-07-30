#version 330 core
layout (location=0) in vec2 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 texCoord;

uniform mat4 matModel;

layout (std140) uniform mat{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

void main(){
    gl_Position=proj*matModel*vec4(aPos,0,1);
    texCoord=aTexCoord;
}
