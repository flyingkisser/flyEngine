#version 330 core
precision highp float;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture1;
uniform vec3 textColor;

void main(){
    //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, texCoord).r);
    //FragColor = vec4(textColor, 1.0) * sampled;

    FragColor = vec4(textColor, 1.0)*texture(texture1, texCoord).r;
}
