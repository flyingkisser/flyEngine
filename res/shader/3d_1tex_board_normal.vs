#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;
layout (location=3) in vec3 aTangent;
layout (location=4) in vec3 aBitangent;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

out VS_OUT {
    vec3 fragPos;
    vec2 texCoord;
    mat3 TBN;
    mat3 TBNReverse;
    vec3 cam_pos;
} vs_out;

uniform mat4 matModel;

void main(){
    gl_Position=proj*view*matModel*vec4(aPos,1);
    vs_out.fragPos=vec3(matModel*vec4(aPos,1));
    vs_out.texCoord=aTexCoord;
    vs_out.cam_pos=cam_pos;

    // mat4 mat4Model = transpose(inverse(matModel));
    // vec3 T = normalize(vec3(mat4Model * vec4(aTangent,   0.0)));
    // vec3 B = normalize(vec3(mat4Model * vec4(aBitangent, 0.0)));
    // vec3 N = normalize(vec3(mat4Model * vec4(aNormal,    0.0)));

    mat3 mat3Model = transpose(inverse(mat3(matModel)));
    vec3 T = normalize(mat3Model * aTangent);
    vec3 B = normalize(mat3Model * aBitangent);
    vec3 N = normalize(mat3Model * aNormal);
    vs_out.TBN=mat3(T,B,N);
    vs_out.TBNReverse=transpose(vs_out.TBN);
}
