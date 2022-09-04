#version 330 core
precision highp float;
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform float mixValue;

void main(){
    //first*(1-mixValue)+second*mixValue
    vec4 texColor1=texture(texture0,texCoord);
    vec4 texColor2=texture(texture1,texCoord);
    if(texColor2.a==0.0){
        FragColor=texColor1;
    }else{
        FragColor=mix(texture(texture0,texCoord),texture(texture1,texCoord),mixValue);
    }
 
}
