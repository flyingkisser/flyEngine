#version 330 core
layout(location=0) in vec3 aPos;

out vec3 texCoords;

layout (std140) uniform mat{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;

void main(){
    vec4 pos=proj*view*vec4(aPos,1.0);
    gl_Position = pos.xyww;
    texCoords=aPos;
}
