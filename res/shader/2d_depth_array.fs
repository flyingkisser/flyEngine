#version 330 core
precision highp float;

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2DArray texture0;
uniform bool bFlipX;
uniform bool bFlipY;
uniform int lightIndex;
void main(){
    float x=texCoord.x;
    float y=texCoord.y;
    if(bFlipX==true)
        x=1.0f-texCoord.x;
    if(bFlipY==true)
        y=1.0f-texCoord.y;
    vec2 pos=vec2(x,y);
    float depthValue=texture(texture0,vec3(pos,lightIndex)).r;
    FragColor=vec4(depthValue,depthValue,depthValue,1.0f);
}
