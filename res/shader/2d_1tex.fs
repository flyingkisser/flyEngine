#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform bool bFlipX;
uniform bool bFlipY;
uniform bool bReverseColor;
uniform bool bGray;
void main(){
    float x=texCoord.x;
    float y=texCoord.y;
    if(bFlipX==true)
        x=1-texCoord.x;
    if(bFlipY==true)
        y=1-texCoord.y;
    vec2 pos=vec2(x,y);
    vec4 color=texture(texture0,pos);
    if(bReverseColor)
        FragColor=vec4(vec3(1.0-color),1.0);  //颜色反转
    else if(bGray){
        float r=0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
        FragColor=vec4(r,r,r,1.0);
    }
    else
        FragColor=color;
}
