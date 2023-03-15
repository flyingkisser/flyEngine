#version 330 core
precision highp float;
in float height;
out vec4 FragColor;

void main(){
    float h=(height+16.0)/64.0f;
    FragColor=vec4(h,h,h,1.0);
}
