#version 330 core
//指定默认的精度限定符
//highp     float(-2^62, 2^62),int(-2^16, 2^16) 32位, 适用于顶点变换
//mediump   float(-2^14, 2^14),int(-2^10, 2^10) 16位, 适用于uv坐标，比highp快2倍
//lowp      float(-2, 2),int(-2^8, 2^8)         10位, 适用于颜色，照明和其他高性能计算，是highp的4倍
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

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

out vec4 FragColor;
uniform sampler2D texture0;

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
    g_normal_vector=normalize(normalVector);
    g_view_vector=normalize(uni_cam_pos-posFrag);
    vec4 obj_color=texture(texture0,texCoord);
    FragColor=obj_color;

    for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,p.specular);
    }
  
    //如果物体的材质启用了，要计算物体本身的材质对于光照的影响1
    FragColor=vec4(g_ambient+g_diffuse+g_specular,1)*obj_color;
}
