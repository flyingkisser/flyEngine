#version 410 core
precision highp float;

out vec4 FragColor;
in vec2 texCoord;
uniform vec4 color;
// uniform sampler2D heightMap;

void main(){
    // float h=texture(heightMap,texCoord).y;
    FragColor=vec4(vec3(color),1.0);
}