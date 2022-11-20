#version 330 core
precision highp float;
in vec2 texCoord;
in vec4 posProj;
out vec4 FragColor;

uniform sampler2D texture0;

void main(){
    vec3 pos=posProj.xyz/posProj.w;
    pos=pos*0.5+0.5;
    // FragColor=texture(texture0,pos.xy);
    FragColor=texture(texture0,texCoord);
}
