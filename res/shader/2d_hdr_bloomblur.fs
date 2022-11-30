#version 330 core
precision highp float;

in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

void main()
{             

	// FragColor = vec4(texture(scene, texCoord).rgb, 1.0);
	// FragColor = vec4(texture(bloomBlur, texCoord).rgb, 1.0);
	// return;

    const float gamma = 2.2;
    vec3 hdrColor = texture(scene, texCoord).rgb;      
    vec3 bloomColor = texture(bloomBlur, texCoord).rgb;
    hdrColor += bloomColor; // additive blending
    // tone mapping
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // also gamma correct while we're at it       
    // vec3 result=hdrColor;
    // result = pow(result, vec3(1.0 / gamma));
    FragColor = vec4(result, 1.0);
}  

