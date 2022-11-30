#version 330 core
precision highp float;

in vec2 texCoord;
out vec4 FragColor;
  
uniform sampler2D texture0;
  
uniform bool horizontal;
uniform float weight[5]=float[](0.227027,0.1945946,0.1216216,0.054054,0.016216);

void main(){
    vec2 texInner=1.0/textureSize(texture0,0); // gets size of single texel
    float xoffset=texInner.x;
    float yoffset=texInner.y;
    vec3 result=texture(texture0,texCoord).rgb*weight[0]; // current fragment's contribution
    if(horizontal){
        for(int i=1;i<5;++i){
            result+=texture(texture0,texCoord+vec2(xoffset*i,0.0)).rgb*weight[i];
            result+=texture(texture0,texCoord-vec2(xoffset*i,0.0)).rgb*weight[i];
        }
    }else{
        for(int i=1;i<5;++i){
            result+=texture(texture0,texCoord+vec2(0.0,yoffset*i)).rgb*weight[i];
            result+=texture(texture0,texCoord-vec2(0.0,yoffset*i)).rgb*weight[i];
        }
    }
    FragColor = vec4(result, 1.0);
}
