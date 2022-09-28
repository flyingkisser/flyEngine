#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices=1000) out;

in VS_OUT{
	vec2 texCoord;
	vec3 normalVector;
	vec3 posFrag;
	vec3 uni_cam_pos;
} vsIn[];

out vec2 texCoord;
out vec3 normalVector;
out vec3 posFrag;
out vec3 uni_cam_pos;

uniform float time;
uniform int gen_max;

void genOneVertex(int index,vec3 offset){
	gl_Position = gl_in[index].gl_Position+vec4(offset,0.0f);
	texCoord = vsIn[index].texCoord;
	normalVector=vsIn[index].normalVector;
	posFrag=vsIn[index].posFrag;
	uni_cam_pos=vsIn[index].uni_cam_pos;
	EmitVertex();
}

void gen(int index){
	float x=0.0,y=0.0,z=0.0;
	float inner=1.5;
	x+=inner*index;
	y+=inner*index;
	z-=inner*index;
    genOneVertex(0,vec3(x,y,z));
    genOneVertex(1,vec3(x,y,z));
    genOneVertex(2,vec3(x,y,z));
    EndPrimitive();
}

void main() {
	int max=gen_max;
	for(int i=0;i<max;i++){
		gen(i);
	}

	// float x=0.0,y=0.0,z=0.0;
    // genOneVertex(0,vec3(x,y,z));
    // genOneVertex(1,vec3(x,y,z));
    // genOneVertex(2,vec3(x,y,z));
    // EndPrimitive();
    // x=3.8;
    // y=3.8;
    // z=-3.8;
    // genOneVertex(0,vec3(x,y,z));
    // genOneVertex(1,vec3(x,y,z));
    // genOneVertex(2,vec3(x,y,z));
    // EndPrimitive();
    // x=5.8;
    // y=5.8;
    // z=-5.8;
    // genOneVertex(0,vec3(x,y,z));
    // genOneVertex(1,vec3(x,y,z));
    // genOneVertex(2,vec3(x,y,z));
    // EndPrimitive();
}