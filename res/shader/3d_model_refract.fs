#version 330 core
precision highp float;
in vec3 posFrag;
in vec3 normalVector;

out vec4 FragColor;
uniform vec3 camera_pos;
uniform samplerCube texture0;

layout (std140) uniform mat{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

void main(){
    vec3 viewDir=normalize(posFrag-camera_pos);
    float f=1.0/1.63;
    vec3 refractDir=refract(viewDir,normalize(normalVector),f);
    FragColor=vec4(texture(texture0,refractDir).rgb,1.0f);
}