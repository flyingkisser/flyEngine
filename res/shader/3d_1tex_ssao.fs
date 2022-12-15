#version 330 core
layout(location=0) out vec3 gPosition;
layout(location=1) out vec3 gNormal;
layout(location=2) out vec4 gAlbedoSpec;

in vec2 texCoord;
in vec3 normalVector;
in vec3 normalVectorInView;
in vec3 posFrag;
in vec3 posFragInView;
in vec3 uni_cam_pos;

uniform sampler2D texture0;

void main(){
	gPosition=posFrag;	//相机视角下的坐标
	gAlbedoSpec.rgb=texture(texture0,texCoord).rgb;
	gAlbedoSpec.a=1.0;
	gNormal=normalize(normalVector);	//相机视角下的法向量
}
