#version 330 core
in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
out vec4 FragColor;

uniform sampler2D texture0;

uniform vec3  camera_pos;

uniform vec3  global_ambient_color;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    sampler2D specular_tex;
    float shininess;
};
uniform Material mt;

struct DirectionLight{
    bool enabled;
    vec3 direction;
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
    
    float cutOffCos;
};

#define POINT_LIGHTS_NUM_MAX 4

uniform PointLight light_point_arr[POINT_LIGHTS_NUM_MAX];

uniform DirectionLight light_direction;

float calcAttenuation(vec3 posFrag,vec3 posLight,float constant,float linear,float quadratic){
    float d    = length(posLight - posFrag);
    float attenuation = 1.0 / (constant + linear * d +
                    quadratic * (d * d));
    return attenuation;
}

void main(){
    vec4 obj_color=texture(texture0,texCoord);
    vec3 normal_vector=normalize(normalVector);
    vec3 view_vector=normalize(camera_pos-posFrag);
    vec3 zero_vector=vec3(0,0,0);
    vec3 ambient=vec3(0,0,0);
    vec3 diffuse=vec3(0,0,0);
    vec3 specular=vec3(0,0,0);
    
    //环境光 平行光(太阳)
    if(light_direction.enabled){
       //全局光照
       ambient+=light_direction.color*light_direction.ambient;
       //漫反射
       vec3 light_vector=normalize(-light_direction.direction);
       diffuse+=light_direction.color*light_direction.diffuse*max(dot(normal_vector,light_vector),0);
       //镜面反射
       vec3 reflect_vector=reflect(-light_vector,normal_vector);
    specular+=light_direction.color*light_direction.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess);
    }
    //点光源
    for(int i=0;i<POINT_LIGHTS_NUM_MAX;i++){
        PointLight light=light_point_arr[i];
        if(!light.enabled)
            continue;
        //环境光
        ambient+=light.color*light.ambient*mt.ambient;
        bool bUseSpot=false;
        bool binsideSpot=false;
        vec3 light_vector=normalize(light.pos-posFrag);
        
        if(light.cutOffCos){
            bUseSpot=true;
            vec3 spotDir=normalize(-light.direction);
            float angle=dot(light_vector,spotDir);
            if(angle>light.cutOffCos)
                binsideSpot=true;
        }
        //如果没有使用探照灯或者片元处于探照灯的范围内
        if(!bUseSpot || (bUseSpot && binsideSpot)){
            //漫反射
            diffuse+=light.color*light.diffuse*max(dot(normal_vector,light_vector),0)*mt.diffuse;
            //镜面反射
            vec3 reflect_vector=reflect(-light_vector,normal_vector);
            specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess)*texture(mt.specular_tex,texCoord).rgb;

            //衰减计算
            if(light.constant!=0){
                float attenuation=calcAttenuation(posFrag,light.pos,light.constant,light.linear,light.quadratic);
                ambient*=attenuation;
                diffuse*=attenuation;
                specular*=attenuation;
            }
        }
       
    }

    FragColor=vec4(ambient+diffuse,1)*obj_color+vec4(specular,0);
}
