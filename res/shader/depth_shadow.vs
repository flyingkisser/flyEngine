#version 330 core
layout (location=0) in vec3 aPos;

layout (std140) uniform mat3d_shadow{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;
// uniform mat4 proj;
// uniform mat4 view;

void main(){
    gl_Position = proj * view * matModel * vec4(aPos, 1);
}
