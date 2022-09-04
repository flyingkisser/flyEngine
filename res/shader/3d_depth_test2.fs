#version 330 core
precision highp float;
out vec4 FragColor;

void main(){
    float near=0.1f;
    float far=100.0f;
    float ndc=gl_FragCoord.z*2.0f-1.0f;
    float linearDepth=(2.0f*near*far)/(far+near-ndc*(far-near));
    FragColor=vec4(vec3(linearDepth/far),1);
}
