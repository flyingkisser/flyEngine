#version 410 core
precision highp float;

#define POINT_LIGHTS_NUM 4
struct PointLight{
    bool enabled;
    vec3 pos;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float radius;
};
layout (std140) uniform light_point{
    PointLight   light_point_arr[POINT_LIGHTS_NUM];
};



out vec4 FragColor;
in vec4 clipSpace;
in vec2 texCoord;
in vec3 posFrag;
in vec3 uni_cam_pos;
uniform float far_plane;
uniform float near_plane;

uniform float moveFactor;
uniform vec4 color;
uniform sampler2D texReflection;
uniform sampler2D texReftraction;
uniform sampler2D texDuDv;
uniform sampler2D texNormal;
uniform sampler2D texDepth;

uniform vec3 waterNormal;

float blurStrength=0.003;
vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);

void checkPoint(vec3 pos,vec3 color,vec3 light_ambient,vec3 light_diffuse,vec3 light_specular){
    vec3 light_vector=normalize(pos-posFrag);
    vec3 ambient=color*light_ambient;
    vec3 diffuse=color*light_diffuse*max(dot(light_vector,g_normal_vector),0.0f);
    float shiness=1.0;
    vec3 halfway_vector=normalize(light_vector+g_view_vector);
    vec3 specular=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),shiness);
    g_ambient+=ambient;
    g_diffuse+=diffuse;
    g_specular+=specular;
}

void main(){
    vec3 g_view_vector=normalize(uni_cam_pos-posFrag);
    vec2 ndc=(clipSpace.xy/clipSpace.w)/2.0+0.5;
    vec2 reflectionTexCoords=vec2(ndc.x,-ndc.y);
    vec2 refractionTexCoords=vec2(ndc);
    
    // vec2 blur1=texture(texDuDv,vec2(texCoord.x-moveFactor,texCoord.y)).rg*2.0-1.0;
    // vec2 blur2=texture(texDuDv,vec2(-texCoord.x+moveFactor,texCoord.y+moveFactor)).rg*2.0-1.0;
    // vec2 blur=(blur1+blur2)/2.0;

    vec2 blur1=texture(texDuDv,vec2(texCoord.x+moveFactor,texCoord.y)).rg*0.1;
    blur1=texCoord+vec2(blur1.x,blur1.y+moveFactor);
    vec2 blur=texture(texDuDv,blur1).rg*2.0-1.0;
    blur*=blurStrength;

    reflectionTexCoords+=blur;
    refractionTexCoords+=blur;

    refractionTexCoords=clamp(refractionTexCoords,0.001,0.999);
    reflectionTexCoords.x=clamp(reflectionTexCoords.x,0.001,0.999);
    reflectionTexCoords.y=clamp(reflectionTexCoords.y,-0.999,-0.001);

    vec3 rgb=texture(texNormal,blur1).rgb;
    vec3 normal=vec3(rgb.r*2-1,rgb.b,rgb.g*2-1);
    normal=normalize(normal);
    g_normal_vector=normal;

    float reftractiveFactor=dot(g_view_vector,g_normal_vector);
    reftractiveFactor=pow(reftractiveFactor,0.5);

    vec4 c1=texture(texReflection,reflectionTexCoords);
    vec4 c2=texture(texReftraction,refractionTexCoords);
    // vec4 outColor=mix(c1,c2,0.5);
    vec4 outColor=mix(c1,c2,reftractiveFactor);
    outColor=mix(outColor,vec4(0,0.3,0.5,1.0),0.2);
    // FragColor=outColor;

    float depth=texture(texDepth,refractionTexCoords).r;
    float floorDistance=2.0*near_plane*far_plane/(far_plane+near_plane-(2*depth-1)*(far_plane-near_plane));

    depth=gl_FragCoord.z;
    float waterDistance=2.0*near_plane*far_plane/(far_plane+near_plane-(2*depth-1)*(far_plane-near_plane));
    float waterDepth=floorDistance-waterDistance;

     for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,p.specular);
    }

    outColor+=vec4(g_specular,0.0);
    //水深为0的地方，完全透明，深度超过5时，完全不透明，在水的边缘处过度的更加自然
    //当前代码中，地板是平的，水的深度也是一样的值，暂时不使用
    // outColor.a=clamp(waterDepth/0.1,0,1);
    FragColor=outColor;
    // FragColor=vec4(waterDistance/50.0,waterDistance/50.0,waterDistance/50.0,1.0);

}