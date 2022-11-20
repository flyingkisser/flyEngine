#version 330 core
layout(location=0) in vec3 aPos;

out vec3 tex_vector;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;

void main(){
    vec4 pos=proj*view*vec4(aPos,1.0);
    gl_Position = pos.xyww;
    tex_vector=aPos;
}
