#version 410 core
layout (triangles, invocations=5) in;
layout (triangle_strip, max_vertices=3) out;
uniform mat4 lightSpaceMatrices[16];
uniform mat4 proj;

void main(){
	for(int i=0;i<3;i++){
		gl_Layer=gl_InvocationID;
		gl_Position=proj*lightSpaceMatrices[gl_InvocationID]*gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}