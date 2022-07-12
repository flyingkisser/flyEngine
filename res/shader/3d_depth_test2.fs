#version 330 core
out vec4 FragColor;

void main(){
    float near=0.1;
    float far=100;
    float ndc=gl_FragCoord.z*2-1;
    float linearDepth=(2*near*far)/(far+near-ndc*(far-near));
    FragColor=vec4(vec3(linearDepth/far),1);
}
