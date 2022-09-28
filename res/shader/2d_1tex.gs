#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices=1000) out;

in vec2 texCoord_gs[];
out vec2 texCoord;


void gen(int index,vec3 offset){
	gl_Position = gl_in[index].gl_Position+vec4(offset,0);
	texCoord=texCoord_gs[index];
	EmitVertex();
}

void main() {
   	vec3 offset=vec3(0,0,0);
    gen(0,offset);
    gen(1,offset);
    gen(2,offset);
    EndPrimitive();	
    offset=vec3(0.5,0.5,0);
    gen(0,offset);
    gen(1,offset);
    gen(2,offset);
    EndPrimitive();

	// offset=vec3(0.2,0.2,0);
    // gen(0,offset);
    // gen(1,offset);
    // gen(2,offset);
    // EndPrimitive();
    // offset=vec3(0.3,0.3,0);
    // gen(0,offset);
    // gen(1,offset);
    // gen(2,offset);
    // EndPrimitive();
    // offset=vec3(0.4,0.4,0);
    // gen(0,offset);
    // gen(1,offset);
    // gen(2,offset);
    // EndPrimitive();
    // offset=vec3(0.5,0.5,0);
    // gen(0,offset);
    // gen(1,offset);
    // gen(2,offset);
    // EndPrimitive();
}