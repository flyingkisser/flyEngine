#version 330 core
layout (location=0) in vec3 aPos;
out float height;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};
// uniform mat4 proj;
// uniform mat4 view;
uniform mat4 matModel;

void main(){
    height=aPos.y;
    gl_Position = proj * view * matModel * vec4(aPos, 1);
}