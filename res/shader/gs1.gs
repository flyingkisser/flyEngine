#version 330 core
precision highp float;
layout (points) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 color;
} vsIn[];

out vec3 fColor;

void main() {    
    vec4 pos=gl_in[0].gl_Position;
    fColor=vsIn[0].color;
    gl_Position = pos+vec4(-0.2,-0.2,0,0); 
    EmitVertex();
    gl_Position = pos+vec4(0.2,-0.2,0,0); 
    EmitVertex();
    // fColor=vec3(1.0,0.0,0.0);
    gl_Position = pos+vec4(-0.2,0.2,0,0); 
    EmitVertex();
    EndPrimitive();
}