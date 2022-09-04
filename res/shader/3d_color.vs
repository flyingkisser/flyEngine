#version 330 core

layout (location=0) in vec3 aPos;

uniform mat4 matModel;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

void main(){
    gl_Position = proj * view * matModel * vec4(aPos, 1);
}
