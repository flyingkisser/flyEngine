#version 330 core
precision highp float;

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;
uniform float bloomFactor=0.0f;
const float gamma = 2.2;

void main()
{             
    //因为最终的结果，片元和bloom的总是上下对称的，需要把片元的y坐标对称
    vec3 hdrColor = texture(scene, vec2(texCoord.x,1.0f-texCoord.y)).rgb;      
    vec3 bloomColor = texture(bloomBlur, texCoord).rgb;
    // if(bloomFactor<=0.0f)
    //     hdrColor += bloomColor; // additive blending
    // else
    //     hdrColor=mix(hdrColor,bloomColor,bloomFactor);  //linear interpolation
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result,1.0);
}  

