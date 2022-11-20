#version 330 core
precision highp float;

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform bool bFlipX;
uniform bool bFlipY;
void main(){
    float x=texCoord.x;
    float y=texCoord.y;
    if(bFlipX==true)
        x=1.0f-texCoord.x;
    if(bFlipY==true)
        y=1.0f-texCoord.y;
    vec2 pos=vec2(x,y);
    float depthValue=texture(texture0,pos).r;
    FragColor=vec4(depthValue,depthValue,depthValue,1.0f);
}
