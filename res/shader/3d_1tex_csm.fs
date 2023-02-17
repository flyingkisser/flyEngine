#version 410 core
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
in mat4 cam_view;

out vec4 FragColor;
uniform bool g_debug;
uniform float far_plane;
uniform float gamma_correct;
uniform bool bUseBlinnPhong;
uniform bool texture_specular_mesh_0_enabled;

uniform sampler2D texture0;
uniform Material mt;
uniform sampler2D texture_specular_mesh_0;
uniform sampler2DArray texture_shadow_array;
uniform mat4 lightSpaceMatrices[4];
uniform int shadowLevelCount;
uniform float shadowLevelDistance[16];

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

float isInShadowPCF(vec3 fragPosWorldSpace,vec3 light_dir){
    float shadow=0.0f;
    vec4 fragPosViewSpace=cam_view*vec4(fragPosWorldSpace,1.0);
    float depthValue=abs(fragPosViewSpace.z);
    int layer=-1;
    for(int i=0;i<shadowLevelCount;i++){
        if(depthValue<shadowLevelDistance[i]){
            layer=-1;
            break;
        }
    }
    if(layer==-1)
        layer=shadowLevelCount;
    vec4 fragPosLightSpace=lightSpaceMatrices[layer]*vec4(fragPosWorldSpace,1.0);
    //转换到[-1,1]的normalized device 坐标空间，即NDC坐标
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    //如果片元z大于1.0f说明片元映射到光照空间坐标中，位于远平面更远处，这里返回false，即认为不在阴影中，则不会出现远平面更远处的大片黑色
    if(projCoords.z>1.0f)
        return 0.0f;
    //再转到 [0,1] 区间，即纹理的uv坐标空间
    projCoords = projCoords * 0.5 + 0.5;
    float bias=max(0.05*(1.0-dot(g_normal_vector,light_dir)),0.005);
    float biasModifier=0.5;
    if(layer==shadowLevelCount)
        bias*=1/(far_plane*biasModifier);
    else
        bias*=1/(shadowLevelDistance[layer]*biasModifier);

    vec2 texelSize=1.0/vec2(textureSize(texture_shadow_array,0));
    for(int x=-1;x<=1;x++){
        for(int y=-1;y<=1;y++){
            float pcfDepth=texture(texture_shadow_array,
                vec3(projCoords.xy+vec2(x,y)*texelSize,layer)).r;
            shadow+=projCoords.z-bias>pcfDepth?1.0:0.0;
        }
    }
    shadow/=9.0f;
    return shadow;
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
        // vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
        // specular+=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),light_shiness);

        float shiness=light_shiness;
        vec3 sp;
        if(!bUseBlinnPhong){
            vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
            sp=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),shiness);
        }
        else{
            vec3 halfway_vector=normalize(light_vector+g_view_vector);
            sp=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),shiness);
        }
        specular+=sp;

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
    // if(mt.enabled && mt.shiness>0.0f){ //镜面反射
    //     vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
    //     vec3 sp=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),mt.shiness);
    //     if(mt.sp_tex_enabled)
    //         sp*=texture(mt.specular_tex,texCoord).rgb;
    //     specular+=sp;
    // }

    float shiness=1.0;
    if(mt.enabled && mt.shiness>=0.0f)
        shiness=mt.shiness;
    vec3 sp;
    if(!bUseBlinnPhong){
        vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
        sp=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),shiness);
        if(g_debug)
            g_debug_color=vec3(max(dot(g_view_vector,reflect_vector),0.0f));
    }
    else{
        vec3 halfway_vector=normalize(light_vector+g_view_vector);
        sp=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),shiness);
        if(g_debug)
            g_debug_color=vec3(max(dot(g_normal_vector,halfway_vector),0.0f));
    }

    if(mt.enabled && mt.sp_tex_enabled)
        sp*=texture(mt.specular_tex,texCoord).rgb;
    specular+=sp;

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

    if(l>0.0){
        // 在角度内
        // 漫反射
        diffuse+=color*light_diffuse*max(dot(g_normal_vector,light_vector),0.0f)*l;
        //镜面反射
        // if(mt.enabled && mt.shiness>0.0f){
        //     vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
        //     vec3 sp=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),mt.shiness)*l;
        //     if(mt.sp_tex_enabled)
        //         sp*=texture(mt.specular_tex,texCoord).rgb;
        //     specular+=sp;
        // }

        float shiness=1.0f;
        if(mt.enabled && mt.shiness>=0.0f)
            shiness=mt.shiness;
        vec3 sp;
        // light_specular*=2;
        if(!bUseBlinnPhong){
            vec3 reflect_vector=reflect(-light_vector,g_normal_vector);
            sp=color*light_specular*pow(max(dot(g_view_vector,reflect_vector),0.0f),shiness);
        }
        else{
            vec3 halfway_vector=normalize(light_vector+g_view_vector);
            sp=color*light_specular*pow(max(dot(g_normal_vector,halfway_vector),0.0f),shiness);
        }
        if(mt.enabled && mt.sp_tex_enabled)
            sp*=texture(mt.specular_tex,texCoord).rgb;
        specular+=sp;
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
    // FragColor=obj_color;
    // return;

    float shadowDir=0.0;
    float shadowPoint=0.0;
    float shadowSpot=0.0;
    
    
    for(int i=0;i<DIR_LIGHT_NUM;i++){
        DirectionLight dir=light_dir_arr[i];
        if(!dir.enabled)
            continue;
        light_dirty=true;
        checkDir(dir.direction,dir.color,dir.ambient,dir.diffuse,dir.specular,dir.shiness);
        shadowDir+=isInShadowPCF(posFrag,dir.direction);
    }

    for(int i=0;i<POINT_LIGHTS_NUM;i++){
        PointLight p=light_point_arr[i];
        if(!p.enabled)
            continue;
        light_dirty=true;
        checkPoint(p.pos,p.color,p.ambient,p.diffuse,p.specular,p.constant,p.linear,p.quadratic);
        shadowPoint+=isInShadowPCF(posFrag,normalize(p.pos-uni_cam_pos));
    }

    for(int i=0;i<SPOT_LIGHT_NUM;i++){
        SpotLight p=light_spot_arr[i];
        if(!p.enabled)
            continue;
        light_dirty=true;
        checkSpot(p.cutoff_inner,p.cutoff_outer,p.direction,p.pos,p.color,p.ambient,p.diffuse,p.specular,p.constant,p.linear,p.quadratic);   
        shadowSpot+=isInShadowPCF(posFrag,p.direction);
    }
   
    float shadowValue=shadowDir+shadowPoint+shadowSpot;
    if(shadowValue>1.0)
        shadowValue=1.0;
   
  
    //如果物体的材质启用了，要计算物体本身的材质对于光照的影响1
    if(light_dirty){
        if(mt.enabled){
            FragColor=vec4(mt.ambient*g_ambient+(1.0-shadowValue)*(mt.diffuse*g_diffuse+mt.specular*g_specular),1)*obj_color;
            //如果model模型单独使用的高亮贴图
            if(texture_specular_mesh_0_enabled){
                vec4 spec_color=texture(texture_specular_mesh_0,texCoord);
                FragColor=vec4(mt.ambient*g_ambient+(1.0-shadowValue)*(mt.diffuse*g_diffuse+mt.specular*g_specular),1)*obj_color+(1.0-shadowValue)*vec4(mt.specular*g_specular,1)*spec_color;
            }
        }
        else{
            FragColor=vec4(g_ambient+(1.0-shadowValue)*(g_diffuse+g_specular),1)*obj_color;
            if(texture_specular_mesh_0_enabled){
                vec4 spec_color=texture(texture_specular_mesh_0,texCoord);
                FragColor=vec4(g_ambient+(1.0-shadowValue)*(g_diffuse+g_specular),1)*obj_color+(1.0-shadowValue)*vec4(g_specular,1)*spec_color;
            }
        }
    }
    else{
        FragColor=obj_color;
    }

    //如果启用了gamma校正，需要把颜色返回到线性空间，因为最后显示器会把颜色设置为pow(color,gamma)
    //所以这里较正为pow(color,1.0f/gamma);
    //也有显示器把gamma值设置为1的
    if(gamma_correct>0.0f)
        FragColor.rgb=pow(FragColor.rgb,vec3(1.0f/gamma_correct));
        
    if(g_debug){
         vec3 light2frag = posFrag-light_point_arr[0].pos;
         // float depthValue=texture(texture_depth_cube,light2frag).r*far_plane;
         // return length(light2frag)-0.008 > depthValue;
        // FragColor.rgb=vec3(texture(texture_depth_cube,light2frag).r);
        FragColor.rgb=vec3(shadowValue);
    }

}
