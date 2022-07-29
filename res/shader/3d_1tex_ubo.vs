#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;

out vec2 texCoord;

layout (std140) uniform mat{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 model;

void main(){
    gl_Position=proj*view*model*vec4(aPos,1.0f);
    texCoord=aTexCoord;
}
