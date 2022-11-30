#version 330 core
precision highp float;
layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 BrightColor;
uniform vec3 color;

void main(){
    FragColor=vec4(color,1.0f);
    float brightness = dot(FragColor.rgb, vec3(0.7152, 0.7152, 0.7152));
    if(brightness>50.0)
        BrightColor=vec4(FragColor.rgb,1.0f);
    else
        BrightColor=vec4(0,0,0,1.0);
}
