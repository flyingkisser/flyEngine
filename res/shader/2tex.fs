#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
//uniform float mixValue;

void main(){
    //first*(1-mixValue)+second*mixValue
    FragColor=mix(texture(texture1,texCoord),texture(texture2,texCoord),0.7);
}
