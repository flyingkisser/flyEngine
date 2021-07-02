#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
//uniform sampler2D texture2;
//uniform float mixValue;

void main(){
    //first*(1-mixValue)+second*mixValue
    vec4 color=texture(texture1,texCoord);
    if(color.a<=0.0)
        color.r=1.0;
    //color.a=0.5;
    FragColor=color;
//    FragColor=texture(texture1,texCoord);
}
