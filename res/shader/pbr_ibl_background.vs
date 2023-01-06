#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;
layout (location=2) in vec3 aNormal;

out vec3 localPos;

// layout (std140) uniform mat3d{
//     mat4 proj;
//     mat4 view;
//     vec3 cam_pos;
// };

uniform mat4 proj;
uniform mat4 view;

void main(){
    localPos=aPos;
    vec4 clipPos = proj * mat4(mat3(view)) * vec4(aPos, 1);
    gl_Position=clipPos.xyww;
}