#version 330 core
precision highp float;


struct Material{
    bool enabled;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
    bool sp_tex_enabled;
    sampler2D specular_tex;
};

//环境光 平行光(太阳)
struct DirectionLight{
    bool enabled;   //4  offset 0
    vec3 direction; //12 offset 16
    vec3 color;     //12 offset 32
    float ambient;  //4  offset 44
    float diffuse;  //4  offset 48
    float specular; //4  offset 52
    float shiness;    //4  offset 56
};

//点光源
struct PointLight{
    bool enabled;   //是否启用
    vec3 pos;       //位置
    vec3 color;     //光的颜色

    vec3 ambient;   //环境光材质系数
    vec3 diffuse;   //漫反射材质系数
    vec3 specular;  //镜面反射材质系数
    
    float constant;     //距离衰减常量
    float linear;       //距离衰减一次常量系数
    float quadratic;    //距离衰减二次常量系数
};


uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;
uniform vec3 camera_pos;
uniform Material mt;

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;
out vec4 FragColor;

layout (std140) uniform light_dir{
    DirectionLight light_direction;
};
layout (std140) uniform light_point{
    PointLight light_point_arr[POINT_LIGHT_NUM];
};

float calcAttenuation(vec3 posFrag,vec3 posLight,float constant,float linear,float quadratic){
    float d    = length(posLight - posFrag);
    float attenuation = 1.0 / (constant + linear * d +
                    quadratic * (d * d));
    return attenuation;
}

void main()
{
    vec4 obj_color=texture(texture_diffuse_0,texCoord);
    vec4 spec_color=texture(texture_specular_0,texCoord);
    vec3 normal_vector=normalize(normalVector);
    vec3 view_vector=normalize(uni_cam_pos-posFrag);
    vec3 zero_vector=vec3(0,0,0);
    vec3 ambient=vec3(0,0,0);
    vec3 diffuse=vec3(0,0,0);
    vec3 specular=vec3(0,0,0);
    bool light_dirty=false;
    
    
    if(light_direction.enabled){
        light_dirty=true;
        //环境光
        ambient+=light_direction.color*light_direction.ambient;
        //漫反射
        vec3 light_vector=normalize(-light_direction.direction);
        diffuse+=light_direction.color*light_direction.diffuse*max(dot(normal_vector,light_vector),0.0f);
        //镜面反射
        if(light_direction.shiness>0.0f){
            vec3 reflect_vector=reflect(-light_vector,normal_vector);
            specular+=light_direction.color*light_direction.specular*pow(max(dot(view_vector,reflect_vector),0.0f),light_direction.shiness);
        }
    }
   
    //点光源
    for(int i=0;i<POINT_LIGHT_NUM;i++){
        PointLight light=light_point_arr[i];
        if(!light.enabled)
            continue;
        light_dirty=true;
        vec3 light_vector=normalize(light.pos-posFrag);
        ambient+=light.color*light.ambient;  //环境光
        diffuse+=light.color*light.diffuse*max(dot(normal_vector,light_vector),0.0f);  //漫反射
        if(mt.enabled && mt.shiness>0.0f){ //镜面反射
            vec3 reflect_vector=reflect(-light_vector,normal_vector);
            vec3 sp=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0.0f),mt.shiness);
            if(mt.sp_tex_enabled)
                sp*=texture(mt.specular_tex,texCoord).rgb;
            specular+=sp;
        }
        if(light.constant!=0.0f){  //需要计算距离衰减
            float attenuation=calcAttenuation(posFrag,light.pos,light.constant,light.linear,light.quadratic);
            ambient*=attenuation;
            diffuse*=attenuation;
            specular*=attenuation;
        }
    }
    
    if(light_dirty){
        if(mt.enabled)
            FragColor=vec4(mt.ambient*ambient+mt.diffuse*diffuse+mt.specular*specular,1)*obj_color+vec4(mt.specular*specular,1)*spec_color;
        else
            FragColor=vec4(ambient+diffuse,1)*obj_color+vec4(specular,1)*spec_color;
    }
    else{
        FragColor=obj_color;
    }

    
}
