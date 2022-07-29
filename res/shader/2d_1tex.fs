#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
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
//    FragColor=texture(texture1,vec2(x,y));
    vec2 pos=vec2(x,y);
    if(bReverseColor)
        FragColor=vec4(vec3(1.0-texture(texture1,pos)),1.0);  //颜色反转
    else if(bGray){
        FragColor=texture(texture1, pos);
        float r=0.2126 * FragColor.r + 0.7152 * FragColor.g + 0.0722 * FragColor.b;
        FragColor=vec4(r,r,r,1.0);
    }
    else
        FragColor=texture(texture1,vec2(x,y));
}
