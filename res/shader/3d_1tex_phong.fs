#version 330 core
in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
out vec4 FragColor;

uniform sampler2D texture0;

uniform vec3  camera_pos;

//uniform vec3  global_ambient_color;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
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
};

struct SpotLight{
    bool enabled;
    vec3 pos;
    vec3 color;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
    
    float cutoff;
    vec3  direction;
};

#define POINT_LIGHT_NUM 4
#define SPOT_LIGHT_NUM 4
uniform PointLight light_point_arr[POINT_LIGHT_NUM];
uniform SPOTLight light_spot_arr[POINT_LIGHT_NUM];

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
    
    //环境光
//    if(any(greaterThan(global_ambient_color,zero_vector))){
//        ambient=global_ambient_color*mt.ambient;
//    }
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
    for(int i=0;i<POINT_LIGHT_NUM;i++){
        PointLight light=light_point_arr[i];
        if(!light.enabled)
            continue;
        vec3 light_vector=normalize(light.pos-posFrag);
        
        if(light.cutoff){
            float theta=dot(light_vector,light.direction);
            if(theta>light.cutoff){
                //在角度内
            }else{
                
            }
        }
        //环境光
        ambient+=light.color*light.ambient;
        //漫反射
        diffuse+=light.color*light.diffuse*max(dot(normal_vector,light_vector),0);
        //镜面反射
        vec3 reflect_vector=reflect(-light_vector,normal_vector);
        specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess);
        
        if(light.constant!=0){
            //需要计算距离衰减
            float attenuation=calcAttenuation(posFrag,light.pos,light.constant,light.linear,light.quadratic);
            ambient*=attenuation;
            diffuse*=attenuation;
            specular*=attenuation;
        }
    }

    //聚光灯
    for(int i=0;i<SPOT_LIGHT_NUM;i++){
       SPOTLight light=spot_point_arr[i];
       if(!light.enabled)
           continue;
        if(!light.cutoff)
            continue;
        vec3 light_vector=normalize(light.pos-posFrag);
        float theta=dot(light_vector,light.direction);
        if(theta>light.cutoff){
            //在角度内
            //漫反射
            diffuse+=light.color*light.diffuse*max(dot(normal_vector,light_vector),0);
            //镜面反射
            vec3 reflect_vector=reflect(-light_vector,normal_vector);
            specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess);
        }
        //环境光
        ambient+=light.color*light.ambient;
        if(light.constant!=0){
            //需要计算距离衰减
            float attenuation=calcAttenuation(posFrag,light.pos,light.constant,light.linear,light.quadratic);
            ambient*=attenuation;
            diffuse*=attenuation;
            specular*=attenuation;
        }
    }

    //FragColor=vec4(mt.ambient*ambient+mt.diffuse*diffuse+mt.specular*specular,1)*obj_color;
    FragColor=vec4(mt.ambient*ambient+mt.diffuse*diffuse,1)*obj_color+vec4(mt.specular*specular,0);
}
