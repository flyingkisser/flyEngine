#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;
layout (location=3) in vec3 tagnent;
layout (location=4) in vec3 bitangent;
layout (location=5) in ivec4 boneIDs;
layout (location=6) in vec4 weights;

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
uniform mat4 finalBoneMatrices[100];

void main(){
    vec4 pos=vec4(0.0);
    vec3 normal=aNormal;
    for(int i=0;i<4;i++){
        int boneID=boneIDs[i];
        if(boneID==-1)
            continue;
        if(boneID>=100){
            pos=vec4(aPos,1.0);
            break;
        }
        vec4 localPos=finalBoneMatrices[boneID]*vec4(aPos,1.0);
        pos+=localPos*weights[i];
        normal=mat3(finalBoneMatrices[boneID])*aNormal;
    }
    // pos=finalBoneMatrices[boneIDs[0]]*vec4(aPos,1.0);

    texCoord=aTexCoord;
    normalVector = mat3(transpose(inverse(matModel))) * normal;
    posFrag=vec3(matModel * pos);
    uni_cam_pos=cam_pos;
    gl_Position = proj * view * matModel * pos;
}
