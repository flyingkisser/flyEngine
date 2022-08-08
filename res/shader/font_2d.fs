#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform vec3 textColor;

void main(){
	//code from book
    //vec4 sampled = vec4(1.0, 1.0, 1.0, texture(texture1, texCoord).r);
    //FragColor = vec4(textColor, 1.0) * sampled;
    //code by joe
    float r=texture(texture0,texCoord).r;
	FragColor=vec4(textColor,r);
}
