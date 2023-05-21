#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;


out vec3 view_pos;
out vec2 texCoord;


uniform mat4 proj;
uniform mat4 view;
uniform mat4 matModel;
// uniform sampler2D heightMap;

void main(){
    // float h=texture(heightMap,texCoord).y*64.0-16.0;
    // gl_Position=proj*view*matModel*vec4(aPos.x,aPos.y+h,aPos.z,1);
    gl_Position=proj*view*matModel*vec4(aPos.x,aPos.y-64,aPos.z,1);
    texCoord=aTexCoord;
}
