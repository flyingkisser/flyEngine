#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;
layout (location=2) in vec3 aNormal;

out vec2 texCoord;
out vec3 normalVector;
out vec3 posFrag;
out vec3 uni_cam_pos;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;
uniform vec4 plane;

void main(){
    //所有在平面之上的点都会抛弃掉，即点乘的值为负值的都会裁剪掉
    gl_ClipDistance[0]=dot(matModel * vec4(aPos, 1),plane);
    gl_Position = proj * view * matModel * vec4(aPos, 1);
    posFrag=vec3(matModel * vec4(aPos, 1));
    texCoord=aTexCoord;
    uni_cam_pos=cam_pos;
    normalVector = mat3(transpose(inverse(matModel))) * aNormal;
}
