#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixValue;
uniform vec3 lightColor;

void main(){
    vec3 texColor=mix(texture(texture1,texCoord),texture(texture2,texCoord),mixValue);
    color=vec4(lightColor,1)*texColor;
    FragColor=color;
}

