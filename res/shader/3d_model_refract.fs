#version 330 core
in vec3 posFrag;
in vec3 normalVector;

out vec4 FragColor;
uniform vec3 camera_pos;
uniform samplerCube texture0;

void main(){
    vec3 viewDir=normalize(posFrag-camera_pos);
    float f=1/1.63;
    vec3 refractDir=refract(viewDir,normalize(normalVector),f);
    FragColor=vec4(texture(texture0,refractDir).rgb,1.0f);
}
