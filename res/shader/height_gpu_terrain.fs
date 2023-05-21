#version 330 core
precision highp float;
in float height;
in vec2 uvs;
out vec4 FragColor;
uniform sampler2D tex_terrain;

void main(){
    // float h=(height+16.0)/32.0f;
    vec3 color=texture(tex_terrain,uvs).rgb;
    // FragColor=vec4(h,h,h,1.0);
    // FragColor=vec4(uvs.x/2+0.5,uvs.y/2+0.5,0,1.0);
    FragColor=vec4(color,1.0);
}