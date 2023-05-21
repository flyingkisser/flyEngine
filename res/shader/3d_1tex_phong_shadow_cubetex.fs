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

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

out vec4 FragColor;

uniform float far_plane;
uniform vec3 lightPos;
uniform sampler2D texture0;
uniform samplerCube texture_depth_cube;

vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);
vec3 g_debug_color=vec3(0,0,0);

void checkDir(vec3 direction,vec3 color,float light_ambient,float light_diffuse,float light_specular,float light_shiness){
    g_ambient+=color*light_ambient; //环境光
    vec3 light_vector=normalize(-direction);
    g_diffuse+=color*light_diffuse*max(dot(g_normal_vector,light_vector),0.0f); //漫反射
    vec3 halfway_vector=normalize(light_vector+g_view_vector);
    g_specular+=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),light_shiness);   //镜面反射
}

void checkPoint(vec3 pos,vec3 color,vec3 light_ambient,vec3 light_diffuse,vec3 light_specular){
    vec3 light_vector=normalize(pos-posFrag);
    g_ambient+=color*light_ambient;  //环境光
    g_diffuse+=color*light_diffuse*max(dot(light_vector,g_normal_vector),0.0f);  //漫反射
    vec3 halfway_vector=normalize(light_vector+g_view_vector);
    g_specular+=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),1.0);
}

float isInShadowByCube(vec3 fragPos,vec3 lightPos){
    vec3 light2frag = fragPos-lightPos;
    float depthValue=texture(texture_depth_cube,light2frag).r*far_plane;
    float v=length(light2frag)-0.008 > depthValue ? 1.0 : 0.0;
    return 1.0-v;
}

float isInShadowByCubePCF2(vec3 fragPos,vec3 lightPos){
    if(fragPos.z>=1.0f)
        return 0.0f;

    vec3 sampleOffsetDirections[20] = vec3[](
       vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
       vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
       vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
       vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
       vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
    );   
    vec3 light2frag = fragPos-lightPos;
    float shadow=0;
    int sampleTimes=20;

    for(int i=0;i<sampleTimes;i++){
        float depthValue=texture(texture_depth_cube,light2frag+sampleOffsetDirections[i]*0.005).r*far_plane;
            if(length(light2frag)-0.008 > depthValue)
                shadow+=1.0;
    }
    return 1.0-shadow/(sampleTimes);
}
void main(){
    g_normal_vector=normalize(normalVector);
    g_view_vector=normalize(uni_cam_pos-posFrag);
    bool light_dirty=false;
    vec4 obj_color=texture(texture0,texCoord);
    float shadowValue=isInShadowByCubePCF2(posFrag,lightPos);
    for(int i=0;i<DIR_LIGHT_NUM;i++){
        DirectionLight dir=light_dir_arr[i];
        if(!dir.enabled)
            continue;
        light_dirty=true;
        checkDir(dir.direction,dir.color,dir.ambient,dir.diffuse,dir.specular,dir.shiness);
    }
     for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        if(!p.enabled)
            continue;
        light_dirty=true;
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,p.specular);
    }
    if(!light_dirty)
        FragColor=obj_color*shadowValue;
    else{
        FragColor=vec4(g_ambient+shadowValue*(g_diffuse+g_specular),1)*obj_color;
    }
}
