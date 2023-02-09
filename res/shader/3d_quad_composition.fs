#version 410 core
precision highp float;

out vec4 FragColor;

uniform sampler2D accum;
uniform sampler2D reveal;
const float EPSILON=0.00001f;

bool isApproximatelyEqual(float a,float b){
	float abs_a=abs(a);
	float abs_b=abs(b);
    return abs(a-b)<=(abs_a<abs_b?abs_b:abs_a)*EPSILON;
}

float max3(vec3 v){
	return max(max(v.x,v.y),v.z);
}

void main(){
  	ivec2 fragCoord=ivec2(gl_FragCoord.xy);
  	float revealage=texelFetch(reveal,fragCoord,0).r;
  	if(isApproximatelyEqual(revealage,1.0f))
  		discard;
  	vec4 accumulation=texelFetch(accum,fragCoord,0);
  	//check if overflow!
  	if(isinf(max3(abs(accumulation.rgb))))
  		accumulation.rgb=vec3(accumulation.a);
  	vec3 average_color=accumulation.rgb/max(accumulation.a,EPSILON);
  	FragColor=vec4(average_color,1.0-revealage);
}