#version 330 core
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texObj;
void main(){
    FragColor=texture(texObj,texCoord);
}
