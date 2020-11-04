#version 330 core
in vec3 vertexColor;
in vec2 texCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;

out vec4 FragColor;

void main(){
    //first*(1-mixValue)+second*mixValue
    FragColor=mix(texture(texture1,texCoord),texture(texture2,texCoord),mixValue);
}
