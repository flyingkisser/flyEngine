#version 330 core
precision highp float;

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2DArray texture0;
uniform bool bFlipX;
uniform bool bFlipY;
uniform bool bReverseColor;
uniform bool bGray;
uniform bool bToneMapping;
uniform float exposure;

vec3 toneMapping(vec3 color){
    if(exposure==0.0f)
        return color/(color+vec3(1.0f));
    else{
        return vec3(1.0) - exp(-color * exposure);
    }
}

void main(){
    float x=texCoord.x;
    float y=texCoord.y;
    if(bFlipX==true)
        x=1.0f-texCoord.x;
    if(bFlipY==true)
        y=1.0f-texCoord.y;
    vec2 pos=vec2(x,y);
    vec4 color=texture(texture0,vec3(pos,111));
    if(bReverseColor)
        FragColor=vec4(vec3(1.0-color),1.0);  //颜色反转
    else if(bGray){
        float r=0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
        FragColor=vec4(r,r,r,1.0);
    }else if(bToneMapping)
        FragColor=vec4(toneMapping(vec3(color)),1.0f);
    else
        FragColor=color;
}
