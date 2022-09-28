#version 330 core
layout (triangles) in;
layout (triangle_strip,max_vertices=3) out;

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

vec3 normal;

vec4 explode(vec4 position, vec3 normal){
    float magnitude = 2.0;
    vec3 direction = normal * ((sin(time) + 1.0) / 2.0) * magnitude; 
    return position + vec4(direction, 0.0);
}

vec3 GetNormal(){
    vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
    vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
    return normalize(cross(a, b));
}

void gen(int index){
	gl_Position = explode(gl_in[index].gl_Position, normal);
	texCoord = vsIn[index].texCoord;
	normalVector=vsIn[index].normalVector;
	posFrag=vsIn[index].posFrag;
	uni_cam_pos=vsIn[index].uni_cam_pos;
	EmitVertex();
}

void main() {    
    // vec3 normal = GetNormal();
    normal=GetNormal();
    gen(0);
    gen(1);
    gen(2);
    EndPrimitive();
}
// 	EmitOnePoint(1,offset);
// 	EmitOnePoint(2,offset);
// 	EndPrimitive();
// }