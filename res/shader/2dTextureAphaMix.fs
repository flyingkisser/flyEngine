#version 330 core
in vec3 vertexColor;
in vec2 texCoord;
uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main(){
    //first*0.8+second*0.2
    FragColor=mix(texture(texture1,texCoord),texture(texture2,texCoord),0.4);
    
    //test1: will reverse y of texture2
//    vec2 texCoord2=vec2(texCoord.x,1-texCoord.y);
//    FragColor=mix(texture(texture1,texCoord),texture(texture2,texCoord2),0.2);
    
    //test2: will reverse x of texture2
//    vec2 texCoord2=vec2(1-texCoord.x,texCoord.y);
//    FragColor=mix(texture(texture1,texCoord),texture(texture2,texCoord2),0.2);
}
