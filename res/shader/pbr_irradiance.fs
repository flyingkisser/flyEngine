#version 330 core
precision highp float;
in vec3 localPos;
out vec4 FragColor;
uniform samplerCube texEnvMap;
void main(){
	const float pi=3.14159265359;
	vec3 N=normalize(localPos);
	vec3 irradiance=vec3(0.0);
	vec3 up=vec3(0,1,0);
	vec3 right=normalize(cross(up,N));
	up=normalize(cross(N,right));
	float sampleInner=0.025;
	float count=0.0;
	for(float phi=0.0;phi<2.0*pi;phi+=sampleInner){
		for(float theta=0.0;theta<0.5*pi;theta+=sampleInner){
			vec3 tangentSample=vec3(sin(theta)*cos(phi),sin(theta)*sin(phi),cos(theta));
			vec3 uv=tangentSample.x*right+tangentSample.y*up+tangentSample.z*N;
			irradiance+=texture(texEnvMap,uv).rgb*cos(theta)*sin(theta);
			count+=1.0;
		}
	}
	irradiance=pi*irradiance/count;
	FragColor=vec4(irradiance,1.0);
}