#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;
uniform sampler2D ourTexture;

void main(){
    FragColor=texture(ourTexture,texCoord);
}
//#version 330 core
//out vec4 FragColor;
//
//in vec3 ourColor;
//in vec2 TexCoord;
//
//// texture sampler
//uniform sampler2D texture1;
//
//void main()
//{
//    FragColor = texture(texture1, TexCoord);
//}
