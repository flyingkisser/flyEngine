#version 330 core
layout(location=0) out vec3 gPosition;
layout(location=1) out vec3 gNormal;
layout(location=2) out vec4 gAlbedoSpec;

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
	gPosition=posFrag;
	gAlbedoSpec.rgb=texture(texture0,texCoord).rgb;
	if(texture_specular_mesh_0_enabled)
		gAlbedoSpec.a=texture(texture_specular_mesh_0,texCoord).r;
	else
		gAlbedoSpec.a=0;

	if(texture_normal_mesh_0_enabled)
		gNormal=texture(texture_normal_mesh_0,texCoord).rgb;
	else
		gNormal=vec3(0,0,0);
	gNormal=normalize(normalVector);
}
