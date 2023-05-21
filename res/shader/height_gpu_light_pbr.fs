#version 330 core
//指定默认的精度限定符
//highp     float(-2^62, 2^62),int(-2^16, 2^16) 32位, 适用于顶点变换
//mediump   float(-2^14, 2^14),int(-2^10, 2^10) 16位, 适用于uv坐标，比highp快2倍
//lowp      float(-2, 2),int(-2^8, 2^8)         10位, 适用于颜色，照明和其他高性能计算，是highp的4倍
precision highp float;
#define DIR_LIGHT_NUM 4
#define POINT_LIGHTS_NUM 4
#define SPOT_LIGHT_NUM 4

struct Material{
    bool enabled;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
    bool sp_tex_enabled;
    sampler2D specular_tex;
};
struct DirectionLight{
    bool enabled;
    vec3 direction;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
    float shiness;
};
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
//聚光灯
struct SpotLight{
    bool enabled;
    float cutoff_inner; //内角的cos值
    float cutoff_outer; //外角的cos值

    vec3 direction;
    vec3 pos;
    vec3 color;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

layout (std140) uniform light_dir{
    DirectionLight   light_dir_arr[DIR_LIGHT_NUM];
};
layout (std140) uniform light_point{
    PointLight   light_point_arr[POINT_LIGHTS_NUM];
};
layout (std140) uniform light_spot{
    SpotLight   light_spot_arr[SPOT_LIGHT_NUM];
};


in vec3 normalVector;
in vec3 posFrag;
uniform vec3 uni_cam_pos;
in vec2 texCoord;

out vec4 FragColor;
// uniform mat4 matLightSpace;
// uniform sampler2D texture0;
// uniform sampler2D texture_shadow;

uniform sampler2D tex_albedo;
uniform sampler2D tex_normal;
uniform sampler2D tex_metallic;
uniform sampler2D tex_roughness;
uniform sampler2D tex_ao;
vec3 albedo;
float metallic;
float roughness;
float ao;


vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);
vec3 g_Lo=vec3(0.0);
const float PI = 3.14159265359;


vec3 getNormalFromMap(){
    vec3 tangentNormal = texture(tex_normal, texCoord).xyz * 2.0 - 1.0;
    vec3 Q1  = dFdx(posFrag);
    vec3 Q2  = dFdy(posFrag);
    vec2 st1 = dFdx(texCoord);
    vec2 st2 = dFdy(texCoord);
    vec3 N   = normalize(normalVector);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);
    return normalize(TBN * tangentNormal);
}

float calcAttenuation(vec3 posFrag,vec3 posLight,float constant,float linear,float quadratic){
    float d    = length(posLight - posFrag);
    return 1.0 / (constant + linear * d + quadratic * (d * d));
}

void checkDir(vec3 direction,vec3 color,float light_ambient,float light_diffuse,float light_specular,float light_shiness){
    vec3 ambient=vec3(0,0,0);
    vec3 diffuse=vec3(0,0,0);
    vec3 specular=vec3(0,0,0);

    ambient+=color*light_ambient;
    //漫反射
    vec3 light_vector=normalize(-direction);
    diffuse+=color*light_diffuse*max(dot(g_normal_vector,light_vector),0.0f);
    //镜面反射
    float shiness=light_shiness;
    vec3 sp;
    vec3 halfway_vector=normalize(light_vector+g_view_vector);
    sp=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),shiness);
    specular+=sp;
    g_ambient+=ambient;
    g_diffuse+=diffuse;
    g_specular+=specular;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
float DistributionGGX(vec3 N, vec3 H, float roughness){
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
    
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = 3.1415926 * denom * denom;
    
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness){
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness){
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
    
    return ggx1 * ggx2;
}

void checkPoint(vec3 pos,vec3 color,vec3 light_ambient,vec3 light_diffuse,vec3 light_specular){
    vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, metallic);

    //light radiance
    vec3 light_vector=normalize(pos-posFrag);
    vec3 H=normalize(g_view_vector+light_vector);
    float distance=length(pos-posFrag);
    float attenuation=1.0/(distance*distance);
    vec3 radiance=color*attenuation;
    // radiance=vec3(300.0)*attenuation;

    //cook-torrance brdf
    vec3 F  = fresnelSchlick(clamp(dot(H,g_view_vector),0.0,1.0), F0);
    float NDF=DistributionGGX(g_normal_vector,H,roughness);
    float G=GeometrySmith(g_normal_vector,g_view_vector,light_vector,roughness);

    vec3 numerator=NDF*G*F;
    float denominator=4*max(dot(g_normal_vector,g_view_vector),0.0)*
    max(dot(g_normal_vector,light_vector),0.0)+0.0001;
    vec3 specular=numerator/denominator;

    vec3 kS=F;  //反射光的能量
    vec3 kD=vec3(1.0)-kS;   //折射光的能量
    kD*=1.0-metallic;   //如果材质是金属，则kD为0

    float NdotL=max(dot(g_normal_vector,light_vector),0);
    g_Lo+=(kD*albedo/3.1415926+specular)*radiance*NdotL;
    // g_debug_color+=(kD*albedo/3.1415926+specular)*radiance*NdotL;
}

void main(){
    g_normal_vector=getNormalFromMap();
    g_view_vector=normalize(uni_cam_pos-posFrag);
    albedo=pow(texture(tex_albedo,texCoord).rgb,vec3(2.2));
    metallic=texture(tex_metallic,texCoord).r;
    roughness=texture(tex_roughness,texCoord).r;
    ao=texture(tex_ao,texCoord).r;

    bool light_dirty=false;
    // vec4 obj_color=texture(texture0,texCoord);
    // // float shadowValue=isInShadowPCF(matLightSpace*vec4(posFrag,1));
    float shadowValue=1;
    // for(int i=0;i<DIR_LIGHT_NUM;i++){
    //     DirectionLight dir=light_dir_arr[i];
    //     if(!dir.enabled)
    //         continue;
    //     light_dirty=true;
    //     checkDir(dir.direction,dir.color,dir.ambient,dir.diffuse,dir.specular,dir.shiness);
    // }
     for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        if(!p.enabled)
            continue;
        light_dirty=true;
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,p.specular);
    }
    if(!light_dirty)
        FragColor=vec4(0,0,0,1.0)*shadowValue;
    else{
        vec3 ambient=vec3(0.03)*albedo*ao;
        vec3 color=ambient+g_Lo;
        color=color/(color+vec3(1.0));  //HDR->LDR;
        color=pow(color,vec3(1.0/2.2)); //gamma correct
        FragColor=vec4(color,1.0);

        // FragColor=vec4(g_ambient+shadowValue*(g_diffuse+g_specular),1)*obj_color;
    }
}