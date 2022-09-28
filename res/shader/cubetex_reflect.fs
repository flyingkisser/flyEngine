#version 330 core
precision highp float;
in vec3 vectorNormal;
in vec3 posFrag;
in vec3 originPos;
in vec2 texCoord;

uniform vec3 camera_pos;
//uniform sampler2D texture0;
uniform samplerCube texture0;

out vec4 FragColor;

void main(){
    vec3 vectorReflect=reflect(normalize(posFrag-camera_pos),normalize(vectorNormal));
    FragColor=vec4(texture(texture0,vectorReflect).rgb,1.0f);
//    FragColor=vec4(texture(texture0,texCoord).rgb,1.0f);
}