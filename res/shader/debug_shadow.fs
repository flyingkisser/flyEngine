#version 330 core
//指定默认的精度限定符
//highp     float(-2^62, 2^62),int(-2^16, 2^16) 32位, 适用于顶点变换
//mediump   float(-2^14, 2^14),int(-2^10, 2^10) 16位, 适用于uv坐标，比highp快2倍
//lowp      float(-2, 2),int(-2^8, 2^8)         10位, 适用于颜色，照明和其他高性能计算，是highp的4倍
precision highp float;


in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

out vec4 FragColor;


uniform mat4 matLightSpace;

uniform sampler2D texture0;
uniform sampler2D texture_shadow;

vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);
vec3 g_debug_color=vec3(0,0,0);

bool isInShadow(vec4 fragPosLightSpace){
    //转换到[-1,1]的normalized device 坐标空间，即NDC坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //如果片元z大于1.0f说明片元映射到光照空间坐标中，位于远平面更远处，这里返回false，即认为不在阴影中，则不会出现远平面更远处的大片黑色
    if(projCoords.z>1.0f)
        return false;
    //再映射到[0,1]区间，即纹理的uv坐标空间
    projCoords = projCoords * 0.5 + 0.5;
    //使用uv坐标从深度纹理中采样
    //当前片元的深度值为projCoords.z
    //所以如果当前片元的z值大于深度值，则说明在阴影中
    return projCoords.z-0.008 > texture(texture_shadow, projCoords.xy).r;
}  

float isInShadowPCF(vec4 fragPosLightSpace){
    float shadow=0.0f;
    float bias=0.008;
    //转换到[-1,1]的normalized device 坐标空间，即NDC坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //如果片元z大于1.0f说明片元映射到光照空间坐标中，位于远平面更远处，这里返回false，即认为不在阴影中，则不会出现远平面更远处的大片黑色
    if(projCoords.z>1.0f)
        return 0.0f;
    //再转到 [0,1] 区间，即纹理的uv坐标空间
    projCoords = projCoords * 0.5 + 0.5;
    vec2 texelSize=1.0/textureSize(texture_shadow,0);
    for(int x=-1;x<=1;x++){
        for(int y=-1;y<=1;y++){
            float lightViewDepth=texture(texture_shadow,projCoords.xy+vec2(x,y)*texelSize).r;
            shadow+=projCoords.z-bias>lightViewDepth?1.0:0.0;
        }
    }
    shadow/=9.0f;
    return 1.0f-shadow;
}  

void main(){
    g_normal_vector=normalize(normalVector);
    g_view_vector=normalize(uni_cam_pos-posFrag);
    vec4 obj_color=texture(texture0,texCoord);
    float shadowValue=isInShadowPCF(matLightSpace*vec4(posFrag,1));
    FragColor=obj_color*shadowValue;
}
