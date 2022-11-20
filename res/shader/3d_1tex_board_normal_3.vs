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
    vec3 cam_pos;
    vec3 light_pos;
    vec2 texCoord;
} vs_out;

uniform mat4 matModel;
uniform vec3 lightPos;

void main(){
    gl_Position=proj*view*matModel*vec4(aPos,1);
    mat3 mat3Model = transpose(inverse(mat3(matModel)));
    vec3 T = normalize(mat3Model * aTangent);
    vec3 B = normalize(mat3Model * aBitangent);
    vec3 N = normalize(mat3Model * aNormal);
    mat3 TBN=transpose(mat3(T,B,N));

    vs_out.fragPos=TBN*vec3(matModel*vec4(aPos,1));
    vs_out.cam_pos=TBN*cam_pos;
    vs_out.light_pos=TBN*lightPos;
    vs_out.texCoord=aTexCoord;
}
