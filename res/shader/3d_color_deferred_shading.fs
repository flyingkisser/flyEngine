#version 330 core
precision highp float;
layout(location=0) out vec3 gPosition;
layout(location=1) out vec3 gNormal;
layout(location=2) out vec4 gAlbedoSpec;

uniform vec3 color;

in vec3 posFrag;
in vec3 uni_cam_pos;

void main(){
    gPosition=posFrag;
    gAlbedoSpec.rgb=color;
    gAlbedoSpec.a=0;
    gNormal=vec3(0,0,0);
    // FragColor=vec4(color,1.0f);
}