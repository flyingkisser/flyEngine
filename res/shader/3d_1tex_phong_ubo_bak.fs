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

// layout (std140) uniform light_dir0{
//     bool dir0_enabled;
//     vec3 dir0_direction;
//     vec3 dir0_color;
//     float dir0_ambient;
//     float dir0_diffuse;
//     float dir0_specular;
//     float dir0_shiness;
// };

layout (std140) uniform light_point0{
    bool point0_enabled;   //是否启用
    vec3 point0_pos;       //位置
    vec3 point0_color;     //光的颜色

    vec3 point0_ambient;   //环境光材质系数
    vec3 point0_diffuse;   //漫反射材质系数
    vec3 point0_specular;  //镜面反射材质系数
    
    float point0_constant;     //距离衰减常量
    float point0_linear;       //距离衰减一次常量系数
    float point0_quadratic;    //距离衰减二次常量系数
};

layout (std140) uniform light_spot0{
    // bool  spot0_enabled;
    // float spot0_cutoff_inner; //内角的cos值
    // float spot0_cutoff_outer; //外角的cos值

    // vec3 spot0_direction;    //方向向量
    // vec3 spot0_pos;
    // vec3 spot0_color;
    float spot0_r;
    float spot0_g;
    float spot0_b;

   
    // vec3 spot0_ambient;
    // vec3 spot0_diffuse;
    // vec3 spot0_specular;

    // float spot0_constant;
    // float spot0_linear;
    // float spot0_quadratic;   
};

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

out vec4 FragColor;

uniform sampler2D texture0;
uniform Material mt;

uniform bool texture_specular_mesh_0_enabled;
uniform sampler2D texture_specular_mesh_0;

uniform float sp0_r;
uniform float sp0_g;
uniform float sp0_b;

vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);

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
    if(light_shiness>0.0f){
        vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
        specular+=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),light_shiness);
    }
    g_ambient+=ambient;
    g_diffuse+=diffuse;
    g_specular+=specular;
}

void checkPoint(vec3 pos,vec3 color,vec3 light_ambient,vec3 light_diffuse,vec3 light_specular,float constant,float linear,float quadratic){
    vec3 ambient=vec3(0,0,0);
    vec3 diffuse=vec3(0,0,0);
    vec3 specular=vec3(0,0,0);
    vec3 light_vector=normalize(pos-posFrag);
    ambient+=color*light_ambient;  //环境光
    diffuse+=color*light_diffuse*max(dot(light_vector,g_normal_vector),0.0f);  //漫反射
    if(mt.enabled && mt.shiness>0.0f){ //镜面反射
        vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
        vec3 sp=color*specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),mt.shiness);
        if(mt.sp_tex_enabled)
            sp*=texture(mt.specular_tex,texCoord).rgb;
        specular+=sp;
    }
    if(constant!=0.0f){  //需要计算距离衰减
        float attenuation=calcAttenuation(posFrag,pos,constant,linear,quadratic);
        ambient*=attenuation;
        diffuse*=attenuation;
        specular*=attenuation;
    }
    g_ambient+=ambient;
    g_diffuse+=diffuse;
    g_specular+=specular;
}

void checkSpot(float cutoff_inner,float cutoff_outer,vec3 direction,vec3 pos,vec3 color,vec3 light_ambient,vec3 light_diffuse,vec3 light_specular,float constant,float linear,float quadratic){
    vec3 ambient=vec3(0,0,0);
    vec3 diffuse=vec3(0,0,0);
    vec3 specular=vec3(0,0,0);

    //环境光
    ambient+=color*light_ambient;
    // if(light.constant>0.0f && light.linear>0.0f && light.quadratic>0.0f && light.cutoff_inner>0.0f && 
    //     light.cutoff_outer>0.0f && light.direction.x>0.0f && light.direction.y>0.0f && light.direction.z>0.0f)
    //     specular=vec3(0,0,0);

    vec3 light_vector=normalize(pos-posFrag);
    vec3 light_direction=normalize(-direction);
    float theta=dot(light_vector,light_direction);
    //i值的计算：分母：内角的cos值-外角的cos值 分子：内角的cos值-当前角的cos值
    //当前角的角度小于内角时，i>1，则正常照亮
    //当前角的角度大于外角时，i为负值，不照亮
    //位于内角和外角之间时，i在(0,1)之间，进行衰减计算
    //最后通过clamp函数把i限制在0到1的范围内
    float l=(theta-cutoff_outer)/(cutoff_inner-cutoff_outer);
    // l=abs(l);
    l=clamp(l,0.0f,1.0f);

    //cutoff_inner 0.86
    //cutoff_outer 0.5

    if(l>0.0){
        // 在角度内
        // 漫反射
        diffuse+=color*light_diffuse*max(dot(g_normal_vector,light_vector),0.0f)*l;
        //镜面反射
        if(mt.enabled && mt.shiness>0.0f){
            vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
            vec3 sp=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),mt.shiness)*l;
            if(mt.sp_tex_enabled)
                sp*=texture(mt.specular_tex,texCoord).rgb;
            specular+=sp;
        }
    }
     
    //距离衰减
    if(constant!=0.0f){
        float attenuation=calcAttenuation(posFrag,pos,constant,linear,quadratic);
        ambient*=attenuation;
        diffuse*=attenuation;
        specular*=attenuation;
    }
    g_ambient+=ambient;
    g_diffuse+=diffuse;
    g_specular+=specular;
}

void main(){
    g_normal_vector=normalize(normalVector);
    g_view_vector=normalize(uni_cam_pos-posFrag);
    vec4 obj_color=texture(texture0,texCoord);
    bool light_dirty=false;

    // if(dir0_enabled){
    //     light_dirty=true;
    //     checkDir(dir0_direction,dir0_color,dir0_ambient,dir0_diffuse,dir0_specular,dir0_shiness);
    // }

    if(point0_enabled){
        light_dirty=true;
        checkPoint(point0_pos,point0_color,point0_ambient,point0_diffuse,point0_specular,point0_constant,point0_linear,point0_quadratic);
    }
    
    // if(spot0_enabled){
    //     light_dirty=true;
    //     checkSpot(spot0_cutoff_inner,spot0_cutoff_outer,spot0_direction,spot0_pos,spot0_color,spot0_ambient,spot0_diffuse,spot0_specular,spot0_constant,spot0_linear,spot0_quadratic);
    // }
  
    // checkSpot(spot0_cutoff_inner,spot0_cutoff_outer,spot0_direction,spot0_pos,spot0_color,spot0_ambient,spot0_diffuse,spot0_specular,spot0_constant,spot0_linear,spot0_quadratic);
    // FragColor=vec4(g_ambient+g_diffuse+g_specular,1)*obj_color;

    // vec3 ambient=spot0_color*spot0_ambient;
    // g_ambient+=ambient;
    // FragColor=vec4(spot0_color,1)*obj_color;
    // FragColor=vec4(sp0_r,sp0_g,sp0_b,1)*obj_color;
    // FragColor=vec4(spot0_b,spot0_b,spot0_b,1);

    // 

    //如果物体的材质启用了，要计算物体本身的材质对于光照的影响
    if(light_dirty){
        if(mt.enabled){
            FragColor=vec4(mt.ambient*g_ambient+mt.diffuse*g_diffuse+mt.specular*g_specular,1)*obj_color;
            //如果单独使用的高亮贴图
            if(texture_specular_mesh_0_enabled){
                vec4 spec_color=texture(texture_specular_mesh_0,texCoord);
                FragColor=vec4(mt.ambient*g_ambient+mt.diffuse*g_diffuse+mt.specular*g_specular,1)*obj_color+vec4(mt.specular*g_specular,1)*spec_color;
            }
        }
        else{
            FragColor=vec4(g_ambient+g_diffuse+g_specular,1)*obj_color;
            if(texture_specular_mesh_0_enabled){
                vec4 spec_color=texture(texture_specular_mesh_0,texCoord);
                FragColor=vec4(g_ambient+g_diffuse+g_specular,1)*obj_color+vec4(g_specular,1)*spec_color;
            }
        }
    }
    else{
        FragColor=obj_color;
    }
  
}
