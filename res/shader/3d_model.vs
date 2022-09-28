#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;

out vec2 texCoord;
out vec3 normalVector;
out vec3 posFrag;
out vec3 uni_cam_pos;

out VS_OUT{
    vec2 texCoord;
    vec3 normalVector;
    vec3 posFrag;
    vec3 uni_cam_pos;
} vsOut;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;

void main(){
    texCoord=aTexCoord;
    normalVector = mat3(transpose(inverse(matModel))) * aNormal;
    posFrag=vec3(matModel * vec4(aPos,1));
    uni_cam_pos=cam_pos;

    vsOut.texCoord=texCoord;
    vsOut.normalVector=normalVector;
    vsOut.posFrag=posFrag;
    vsOut.uni_cam_pos=uni_cam_pos;

    gl_Position = proj * view * matModel * vec4(aPos, 1);
}
