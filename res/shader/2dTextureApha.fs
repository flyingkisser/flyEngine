#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
in vec2 texCoord;
uniform sampler2D ourTexture;

void main(){
//    FragColor=texture(ourTexture,texCoord)*vec4(1.0f,1.0f,1.0f,0.01f);
    FragColor=texture(ourTexture,texCoord)*vec4(vertexColor,1.0);
    FragColor.a=0;
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
