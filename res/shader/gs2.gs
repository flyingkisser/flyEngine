#version 330 core
precision highp float;
layout (points) in;
layout (triangle_strip, max_vertices = 5) out;

in VS_OUT {
    vec3 color;
} vsIn[];

out vec3 fColor;

void main() {    
    vec4 pos=gl_in[0].gl_Position;
    fColor=vsIn[0].color; 
    gl_Position = pos + vec4(-0.2, -0.2, 0.0, 0.0);    // 1:bottom-left   
    EmitVertex();   
    gl_Position = pos + vec4( 0.2, -0.2, 0.0, 0.0);    // 2:bottom-right
    EmitVertex();
    gl_Position = pos + vec4(-0.2,  0.2, 0.0, 0.0);    // 3:top-left
    EmitVertex();
    gl_Position = pos + vec4( 0.2,  0.2, 0.0, 0.0);    // 4:top-right
    EmitVertex();
    gl_Position = pos + vec4( 0.0,  0.4, 0.0, 0.0);    // 5:top
    fColor = vec3(1.0, 1.0, 1.0);
    EmitVertex();
    EndPrimitive();  
}