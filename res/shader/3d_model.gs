#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices=6) out;

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

void EmitOnePoint(int index,vec3 offset){
	vec4 pos=gl_in[index].gl_Position;
	texCoord=vsIn[index].texCoord;
	normalVector=vsIn[index].normalVector;
	posFrag=vsIn[index].posFrag;
	uni_cam_pos=vsIn[index].uni_cam_pos;
	gl_Position=pos+vec4(offset,0);
	EmitVertex();
}

void main(){
	float x=0.2;
	float y=0.2;
	float z=0;
	EmitOnePoint(0,vec3(x,y,z));
	EmitOnePoint(1,vec3(x,y,z));
	EmitOnePoint(2,vec3(x,y,z));

	x=-0.5;
	y=-0.5;
	z=0;
	EmitOnePoint(0,vec3(x,y,z));
	EmitOnePoint(1,vec3(x,y,z));
	EmitOnePoint(2,vec3(x,y,z));

	EndPrimitive();
}