#version 330 core
precision highp float;

in vec2 texCoord;
in vec3 view_pos;

out vec4 FragColor;

uniform sampler2D texPosition;
uniform sampler2D texNormal;
uniform sampler2D texAlbedoSpec;
uniform sampler2D texNoise;

uniform mat4 mat_proj;
uniform mat4 mat_view;

uniform vec3 kernelArr[64];
const vec2 noiseScale = vec2(800.0/4.0, 600.0/4.0); 
//计算出遮蔽值写进单通道red

void main()
{             
     // FragColor=vec4(texture(texNoise,texCoord).rgb,1);
     // return;

    float occlusion=0.0;
    float radius=0.5;
    float bias=0.025;

    vec3 posFragInView=normalize(texture(texPosition,texCoord).rgb);
    vec3 normalInView=normalize(texture(texNormal,texCoord).rgb);
    vec3 noise=normalize(texture(texNoise,texCoord*noiseScale).rgb);
   
    //计算TBN
    vec3 tangent=normalize(noise-normalInView*dot(noise,normalInView));
    vec3 bitangent=cross(normalInView,tangent);
    mat3 TBN=mat3(tangent,bitangent,normalInView);

    for(int i=0;i<64;i++){
        vec3 randomVector=TBN*kernelArr[i];
        vec4 offset=vec4(randomVector,1.0);
        offset=mat_proj*mat_view*offset;
        offset.xyz=offset.xyz/offset.w;
        offset.xyz=offset.xyz*0.5+0.5;
        vec3 samplePos=normalize(texture(texPosition,texCoord+offset.xy).rgb);
        float rangeCheck=smoothstep(0.0,1.0,1/abs(posFragInView.z - samplePos.z));
        occlusion+=(samplePos.z>=posFragInView.z ? 0.2 : 0);
        
        // FragColor=vec4(texture(texAlbedoSpec,texCoord).rgb,1);
        //sampleDepth is always 0
        // FragColor=vec4(samplePos.z*0.5+0.5);
        // FragColor=vec4(occlusion,occlusion,occlusion,1);
        // return;
    }


    // for(int i=0;i<64;i++){
    //     vec3 randomVector=TBN*kernelArr[i];
    //     vec3 offsetVector=posFragInView+randomVector*radius;

    //     vec4 offset=vec4(offsetVector,1.0);
    //     offset=mat_proj*offset;
    //     offset.xyz=offset.xyz/offset.w;
    //     offset.xyz=offset.xyz*0.5+0.5;
    //     vec3 samplePos=normalize(texture(texPosition,offset.xy).rgb);
    //     float sampleDepth=samplePos.z;

    //     float rangeCheck=smoothstep(0.0,1.0,radius/abs(posFragInView.z - sampleDepth));
       
    //     occlusion+=(sampleDepth>=normalize(offsetVector).z ? 1 : 0);
        
    //     // FragColor=vec4(texture(texAlbedoSpec,texCoord).rgb,1);
    //     //sampleDepth is always 0
    //     FragColor=vec4(sampleDepth*0.5+0.5);
    //     return;
    // }

    occlusion=1-occlusion/64;
    FragColor=vec4(occlusion);
	return;
}