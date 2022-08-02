#version 330 core
#define POINT_LIGHT_NUM 4
#define SPOT_LIGHT_NUM 4

struct Material{
    bool enabled;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shiness;
};

//环境光 平行光(太阳)
struct DirectionLight{
    bool enabled;   //4
    vec3 direction; //12
    vec3 color;     //12
    float ambient;  //4
    float diffuse;  //4
    float specular; //4
    int shiness;    //4
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

layout (std140) uniform light_dir{
    DirectionLight light_direction;
};

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

out vec4 FragColor;

uniform sampler2D texture0;
uniform Material mt;

uniform PointLight light_point_arr[POINT_LIGHT_NUM];
uniform SpotLight light_spot_arr[SPOT_LIGHT_NUM];
//uniform DirectionLight light_direction;

//layout (std140) uniform lightPoint{
//    PointLight light_point_arr[POINT_LIGHT_NUM];
//};


float calcAttenuation(vec3 posFrag,vec3 posLight,float constant,float linear,float quadratic){
    float d    = length(posLight - posFrag);
    float attenuation = 1.0 / (constant + linear * d +
                    quadratic * (d * d));
    return attenuation;
}

void main(){
    vec4 obj_color=texture(texture0,texCoord);
    vec3 normal_vector=normalize(normalVector);
    vec3 view_vector=normalize(uni_cam_pos-posFrag);
    vec3 zero_vector=vec3(0,0,0);
    vec3 ambient=vec3(0,0,0);
    vec3 diffuse=vec3(0,0,0);
    vec3 specular=vec3(0,0,0);
    bool light_dirty=false;
  
//环境光
//    if(any(greaterThan(global_ambient_color,zero_vector))){
//        ambient=global_ambient_color*mt.ambient;
//    }

    //平行光(太阳)
    if(light_direction.enabled){
        
        light_dirty=true;
      
        //环境光
        ambient+=light_direction.color*light_direction.ambient;
        //漫反射
          /*
        vec3 light_vector=normalize(-light_direction.direction);
        diffuse+=light_direction.color*light_direction.diffuse*max(dot(normal_vector,light_vector),0);
        //镜面反射
        if(light_direction.shiness!=0){
            vec3 reflect_vector=reflect(-light_vector,normal_vector);
            specular+=light_direction.color*light_direction.specular*pow(max(dot(view_vector,reflect_vector),0),light_direction.shiness);
        }
        */
    }
    
    //点光源
    for(int i=0;i<POINT_LIGHT_NUM;i++){
        PointLight light=light_point_arr[i];
        if(!light.enabled)
            continue;
        light_dirty=true;
        vec3 light_vector=normalize(light.pos-posFrag);
    
        ambient+=light.color*light.ambient;  //环境光
        diffuse+=light.color*light.diffuse*max(dot(normal_vector,light_vector),0);  //漫反射
        if(mt.enabled){ //镜面反射
            vec3 reflect_vector=reflect(-light_vector,normal_vector);
            specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shiness);  
        }
        if(light.constant!=0){  //需要计算距离衰减
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
        light_dirty=true;
        //环境光
        ambient+=light.color*light.ambient;
        
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
            if(mt.enabled){
                vec3 reflect_vector=reflect(-light_vector,normal_vector);
                specular+=light.color*light.specular*pow(max(dot(view_vector,reflect_vector),0),mt.shiness)*l;
            }
        }

        //距离衰减
        if(light.constant!=0){
            float attenuation=calcAttenuation(posFrag,light.pos,light.constant,light.linear,light.quadratic);
            ambient*=attenuation;
            diffuse*=attenuation;
            specular*=attenuation;
        }
    }
    
    //如果物体的材质启用了，要计算物体本身的材质对于光照的影响
    if(light_dirty){
        if(mt.enabled)
            FragColor=vec4(mt.ambient*ambient+mt.diffuse*diffuse+mt.specular*specular,1)*obj_color;
        else
            FragColor=vec4(ambient+diffuse+specular,1)*obj_color;
    }
    else{
        FragColor=obj_color;
    }
  
}
