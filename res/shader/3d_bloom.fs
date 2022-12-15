#version 330 core
layout(location=0) out vec4 FragColor;
layout(location=1) out vec4 BrightColor;


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
};

layout (std140) uniform light_point{
    PointLight   light_point_arr[POINT_LIGHTS_NUM];
};

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

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
  
    for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,p.specular);
    }
    
    FragColor=vec4(g_ambient+g_diffuse+g_specular,1)*obj_color;

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness>200.0)
        BrightColor=vec4(FragColor.rgb,1.0f);
    else
        BrightColor=vec4(0,0,0,1.0);
}
