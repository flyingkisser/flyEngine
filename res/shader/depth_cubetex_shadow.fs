#version 330 core
in vec4 fragPos;

uniform vec3 posLight;
uniform float far_plane;

void main(){
	float distance=length(fragPos.xyz-posLight);
	distance/=far_plane;
	gl_FragDepth=distance;
}