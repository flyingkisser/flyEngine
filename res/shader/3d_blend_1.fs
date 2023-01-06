#version 330 core
precision highp float;
in vec2 texCoord;
out vec4 FragColor;
uniform sampler2D texture0;
void main(){
    vec4 color=texture(texture0,texCoord);
    if(color.a<=0.1)
        discard;
        // FragColor=vec4(1,0,0,0);
    else
        FragColor=color;
}
