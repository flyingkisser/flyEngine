#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;
layout (location=3) in vec3 aTangent;
layout (location=4) in vec3 aBitangent;

out vec2 texCoord;
out vec3 normalVector;
out vec3 posFrag;
out vec3 uni_cam_pos;
out mat3 TBN;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;

void main(){
    gl_Position = proj * view * matModel * vec4(aPos, 1);

    mat3 normalMatrix = transpose(inverse(mat3(matModel)));
    texCoord=aTexCoord;
    normalVector = normalMatrix * aNormal;
    posFrag=vec3(matModel * vec4(aPos,1));
    uni_cam_pos=cam_pos;

    vec3 T = normalize(normalMatrix * aTangent);
    vec3 N = normalize(normalMatrix * aNormal);
    T=normalize(T-dot(T,N)*N);  //保持TBN的正交性(三者相互垂直)
    vec3 B=cross(N,T);
    TBN=mat3(T,B,N);   
}
