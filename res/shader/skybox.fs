#version 330 core
precision highp float;
in vec3 tex_vector;
out vec4 FragColor;
uniform samplerCube tex;

void main(){
    FragColor=texture(tex,tex_vector);
}
