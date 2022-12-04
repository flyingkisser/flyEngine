#version 330 core
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
in vec3 view_pos;
out vec4 FragColor;

uniform sampler2D texPosition;
uniform sampler2D texNormal;
uniform sampler2D texAlbedoSpec;
uniform float exposure;

vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);
vec3 g_debug_color=vec3(0,0,0);
vec3 posFrag;

float calcAttenuation(vec3 posFrag,vec3 posLight,float constant,float linear,float quadratic){
    float d=length(posLight - posFrag);
    return 1.0 / (constant + linear * d + quadratic * (d * d));
}

void checkPoint(vec3 pos,vec3 color,vec3 light_ambient,vec3 light_diffuse,float light_specular){
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

void main()
{             
    posFrag=texture(texPosition,texCoord).rgb;
    g_normal_vector=texture(texNormal,texCoord).rgb;
    g_view_vector=normalize(view_pos-posFrag);
    vec3 obj_color=texture(texAlbedoSpec,texCoord).rgb;
    float specular=texture(texAlbedoSpec,texCoord).a;
    for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        //如果启用了点光源的volumn参数
        if(p.radius>0.0f && length(p.pos-posFrag)>=p.radius){
            float v=p.radius/length(p.pos-posFrag);
            FragColor=vec4(v,v,v,1.0);
            return;
            // continue;
        }
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,specular);
    }
    FragColor=vec4((g_ambient+g_diffuse+g_specular)*obj_color,1.0);
    // PointLight p=light_point_arr[0];
    // vec3 light_vector=normalize(p.pos-posFrag);
    // vec3 diffuse=p.color*p.diffuse*max(dot(light_vector,g_normal_vector),0.0f);
    // FragColor=vec4((diffuse)*obj_color,1.0);
	return;
}  

