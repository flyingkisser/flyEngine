#version 330 core
in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
out vec4 FragColor;

uniform sampler2D texture0;

uniform vec3  camera_pos;

uniform vec3  global_ambient_color;

struct Material{
    bool enabled;
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
};

//聚光灯
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
    
    float cutoff_inner; //内角的cos值
    float cutoff_outer; //外角的cos值
    vec3  direction;    //方向向量
};

#define POINT_LIGHTS_NUM_MAX 4
#define SPOT_LIGHT_NUM 4
uniform PointLight light_point_arr[POINT_LIGHTS_NUM_MAX];
uniform SpotLight light_spot_arr[SPOT_LIGHT_NUM];
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
        if(mt.enabled){
            vec3 reflect_vector=reflect(-light_vector,normal_vector);
            specular+=light_direction.color*light_direction.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess);
        }
    }
    
    //点光源
    for(int i=0;i<POINT_LIGHTS_NUM_MAX;i++){
        PointLight light=light_point_arr[i];
        if(!light.enabled)
            continue;
        vec3 light_vector=normalize(light.pos-posFrag);
        //环境光
        ambient+=light.color*light.ambient;
        //漫反射
        diffuse+=light.color*light.diffuse*max(dot(normal_vector,light_vector),0);
        //镜面反射
        vec3 reflect_vector=reflect(-light_vector,normal_vector);
       
        if(mt.enabled){
        specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess)*texture(mt.specular_tex,texCoord).rgb;
        }
        //衰减计算
        if(light.constant!=0){
           float attenuation=calcAttenuation(posFrag,light.pos,light.constant,light.linear,light.quadratic);
           ambient*=attenuation;
           diffuse*=attenuation;
           specular*=attenuation;
        }
    }
    //聚光灯
    for(int i=0;i<SPOT_LIGHT_NUM;i++){
        SpotLight light=light_spot_arr[i];
        if(!light.enabled)
          continue;
        if(light.cutoff_inner<=0)
           continue;
        vec3 light_vector=normalize(light.pos-posFrag);
        //vec3 light_direction=normalize(-light.direction);
        vec3 light_direction=normalize(light.pos);
        float theta=dot(light_vector,light_direction);
        //i值的计算：分母：内角的cos值-外角的cos值 分子：内角的cos值-当前角的cos值
        //当前角的角度小于内角时，i>1，则正常照亮
        //当前角的角度大于外角时，i为负值，不照亮
        //位于内角和外角之间时，i在(0,1)之间，进行衰减计算
        //最后通过clamp函数把i限制在0到1的范围内
        float l=(theta-light.cutoff_outer)/(light.cutoff_inner-light.cutoff_outer);
        l=clamp(l,0.0,1.0);
        if(l>0.0){
           //在角度内
           //漫反射
           diffuse+=light.color*light.diffuse*max(dot(normal_vector,light_vector),0)*l;
           //镜面反射
           vec3 reflect_vector=reflect(-light_vector,normal_vector);
           //specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess)*l;
            if(mt.enabled){
            specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shininess)*texture(mt.specular_tex,texCoord).rgb*l;
            }
        }

        //环境光
        ambient+=light.color*light.ambient;

        //距离衰减
        if(light.constant!=0){
           float attenuation=calcAttenuation(posFrag,light.pos,light.constant,light.linear,light.quadratic);
           ambient*=attenuation;
           diffuse*=attenuation;
           specular*=attenuation;
        }
    }

//    FragColor=vec4(ambient+diffuse,1)*obj_color+vec4(specular,0);
    
    if(mt.enabled)
       //FragColor=vec4(mt.ambient*ambient+mt.diffuse*diffuse,1)*obj_color+vec4(specular,0);
        FragColor=vec4(mt.ambient*ambient+mt.diffuse*diffuse+specular,1)*obj_color;
    else  
        FragColor=vec4(ambient+diffuse+specular,1)*obj_color;
    
}
