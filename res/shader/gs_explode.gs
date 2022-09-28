#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices=6) out;

in vs_out{
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
	vec3 offset=vec3(0.1,0.1,0);
	int index=0;
	vec4 pos=gl_in[index].gl_Position;
	texCoord=vsIn[index].texCoord;
	normalVector=vsIn[index].normalVector;
	posFrag=vsIn[index].posFrag;
	uni_cam_pos=vsIn[index].uni_cam_pos;
	gl_Position=pos+vec4(offset,0);
	EmitVertex();

	offset=vec3(0.1,0.1,0);
	index=1;
	pos=gl_in[index].gl_Position;
	texCoord=vsIn[index].texCoord;
	normalVector=vsIn[index].normalVector;
	posFrag=vsIn[index].posFrag;
	uni_cam_pos=vsIn[index].uni_cam_pos;
	gl_Position=pos+vec4(offset,0);
	EmitVertex();

	offset=vec3(0.1,0.1,0);
	index=2;
	pos=gl_in[index].gl_Position;
	texCoord=vsIn[index].texCoord;
	normalVector=vsIn[index].normalVector;
	posFrag=vsIn[index].posFrag;
	uni_cam_pos=vsIn[index].uni_cam_pos;
	gl_Position=pos+vec4(offset,0);
	EmitVertex();

	EndPrimitive();
}