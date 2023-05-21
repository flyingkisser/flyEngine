#version 330 core
precision highp float;
in float height;
in vec2 uvs;
out vec4 FragColor;

void main(){
    // float h=(height+16.0)/64.0f;
    float h=height/32.0;
    FragColor=vec4(h,h,h,1.0);
}