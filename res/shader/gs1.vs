#version 330 core
layout (location = 0) in vec2 aPos;
out VS_OUT {
    vec3 color;
} vs_out;

void main()
{
    vs_out.color = vec3(aPos,0.3);
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0); 
}