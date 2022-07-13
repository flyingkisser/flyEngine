#version 330 core

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 textColor;

void main(){
    float r=texture(texture1,texCoord).r;
    FragColor=vec4(textColor*r,1);
}
