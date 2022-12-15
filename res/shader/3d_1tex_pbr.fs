#version 330 core
//指定默认的精度限定符
//highp     float(-2^62, 2^62),int(-2^16, 2^16) 32位, 适用于顶点变换
//mediump   float(-2^14, 2^14),int(-2^10, 2^10) 16位, 适用于uv坐标，比highp快2倍
//lowp      float(-2, 2),int(-2^8, 2^8)         10位, 适用于颜色，照明和其他高性能计算，是highp的4倍
precision highp float;
#define POINT_LIGHTS_NUM 4
#define DIR_LIGHT_NUM 4
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
struct DirectionLight{
    bool enabled;
    vec3 direction;
    vec3 color;
    float ambient;
    float diffuse;
    float specular;
    float shiness;
};

layout (std140) uniform light_dir{
    DirectionLight   light_dir_arr[DIR_LIGHT_NUM];
};
layout (std140) uniform light_point{
    PointLight   light_point_arr[POINT_LIGHTS_NUM];
};

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

out vec4 FragColor;
uniform sampler2D texture0;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);
vec3 g_debug_color=vec3(0,0,0);

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
    denom = PI * denom * denom;
    
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
    vec3 light_vector=normalize(pos-posFrag);
    vec3 H=normalize(g_view_vector+light_vector);
    float distance=length(pos,posFrag);
    float attenuation=1.0/(distance*distance);
    vec3 radiance=color*attenuation;
    vec3 F0 = vec3(0.04); 
    F0      = mix(F0, albedo, metallic);
    vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);
    float NDF=DistributionGGX(g_normal_vector,H,roughness);
    float G=GeometrySmith(g_normal_vector,g_view_vector,light_vector,roughness);
    float numerator=NDF*G*F;
    float denominator=4*max(dot(g_normal_vector,g_view_vector),0.0)*
    max(dot(g_normal_vector,light_vector),0.0)+0.0001;
    vec3 specular=numerator/denominator;

    vec3 ambient=color*light_ambient;
    vec3 diffuse=color*light_diffuse*max(dot(light_vector,g_normal_vector),0.0f);
    // float shiness=1.0;
    // vec3 halfway_vector=normalize(light_vector+g_view_vector);
    // vec3 specular=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),shiness);
    g_ambient+=ambient;
    g_diffuse+=diffuse;
    g_specular+=specular;
}

void main(){
    g_normal_vector=normalize(normalVector);
    g_view_vector=normalize(uni_cam_pos-posFrag);
    vec4 obj_color=texture(texture0,texCoord);
    FragColor=obj_color;

   for(int i=0;i<DIR_LIGHT_NUM;i++){
        DirectionLight p=light_dir_arr[i];
        checkDir(p.direction,p.color,p.ambient,p.diffuse,p.specular,p.shiness);
    }
 
    for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,p.specular);
    }
  
    //如果物体的材质启用了，要计算物体本身的材质对于光照的影响1
    FragColor=vec4(g_ambient+g_diffuse+g_specular,1)*obj_color;
}
