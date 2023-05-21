#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;


out vec3 view_pos;
out vec2 texCoord;
out vec3 posFrag;
out vec3 uni_cam_pos;
layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

// uniform mat4 proj;
// uniform mat4 view;
uniform mat4 matModel;
out vec4 clipSpace;

void main(){
    posFrag=vec3(matModel * vec4(aPos, 1));
    uni_cam_pos=cam_pos;
    clipSpace=proj*view*matModel*vec4(aPos,1);
    gl_Position=proj*view*matModel*vec4(aPos,1);
    texCoord=aTexCoord;
    
    
    // gl_Position=vec4(aPos,1);
}
