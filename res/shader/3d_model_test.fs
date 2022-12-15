#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;


uniform bool texture_specular_mesh_0_enabled;
uniform bool texture_normal_mesh_0_enabled;

uniform sampler2D texture0;
uniform sampler2D texture_specular_mesh_0;
uniform sampler2D texture_normal_mesh_0;

void main(){
	FragColor=vec4(posFrag,1.0);
	vec4 a=texture(texture0,texCoord);
	if(texture_specular_mesh_0_enabled)
		vec4 c=texture(texture_specular_mesh_0,texCoord);
	if(texture_normal_mesh_0_enabled)
		vec4 d=texture(texture_normal_mesh_0,texCoord);
}
