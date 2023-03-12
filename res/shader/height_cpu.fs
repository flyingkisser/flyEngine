#version 330 core
precision highp float;
in float height;
out vec4 FragColor;

void main(){
    float h=(height+16.0)/32.0f;
    //h=0.5;
    FragColor=vec4(h,h,h,1.0);
}
