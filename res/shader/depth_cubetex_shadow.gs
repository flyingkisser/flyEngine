#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;
//每进来一个三角形，需要往6个面都绘制一下，所以是6个三角形共18个顶点
uniform mat4 matViewArr[6];
uniform mat4 proj;

out vec4 fragPos;

void main(){
	//在6个面上分别生成一个三角形
	for(int i=0;i<6;i++){
		gl_Layer=i;
		//三角形的三个顶点
		for(int j=0;j<3;j++){
			fragPos=gl_in[j].gl_Position;
			gl_Position=proj*matViewArr[i]*fragPos;
			EmitVertex();
		}
		EndPrimitive();
	}
}