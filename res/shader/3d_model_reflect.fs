#version 330 core
in vec3 normalVector;
in vec3 posFrag;
out vec4 FragColor;

uniform vec3 camera_pos;
uniform samplerCube texture0;
void main(){
    vec3 r=normalize(posFrag-camera_pos);
    vec3 ref=reflect(r,normalize(normalVector));
    FragColor=vec4(texture(texture0,ref).rgb,1.0f);
}
