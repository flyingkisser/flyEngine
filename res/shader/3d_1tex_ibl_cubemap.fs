#version 330 core
precision highp float;
in vec3 localPos;
out vec4 FragColor;

uniform samplerCube envMap;

void main(){
	vec3 envColor=texture(envMap,localPos).rgb;
	envColor=envColor/(envColor+vec3(1.0));
	envColor=pow(envColor,vec3(1.0/2.2));
	FragColor=vec4(envColor,1.0);
}