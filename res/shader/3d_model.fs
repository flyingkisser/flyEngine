#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in vec3 posFrag;
in vec3 normalVector;

uniform sampler2D texture_diffuse_0;
uniform sampler2D texture_specular_0;

//struct LightDirection{
//    bool enabled;
//    vec3 color;
//    vec3 direction;
//    float factor_ambient;
//};
//
//uniform LightDirection ambient_light;

struct struct_light_direction{
    bool enabled;
    vec3 color;
    vec3 direction;
    float ambient;
    float diffuse;
    float specular;
    int shiness;
};

struct struct_light_pos{
    bool enabled;
    vec3 color;
    vec3 pos;
    float ambient;
    float diffuse;
    float specular;
    int shiness;
};

uniform struct_light_direction light_direction;
uniform struct_light_pos light_point_arr[4];

uniform vec3 camera_pos;

void main()
{
    vec3 ambient=vec3(0,0,0);
    vec3 diffuse=vec3(0,0,0);
    vec3 specular=vec3(0,0,0);
    vec4 color=texture(texture_diffuse_0,texCoord);
    vec4 spec_color=texture(texture_specular_0,texCoord);
    bool light=false;
    if(light_direction.enabled){
        light=true;
        vec3 d=normalize(light_direction.direction);
        ambient=light_direction.color*light_direction.ambient;
        diffuse=light_direction.color*light_direction.diffuse*max(dot(d,normalVector),0);
        if(light_direction.specular!=0){
            vec3 r=normalize(reflect(-d,normalVector)); //reflect函数要求第一个参数d，是从光源位置指向片元，需要需要使用-d
            vec3 v=normalize(camera_pos-posFrag);
            specular=light_direction.color*light_direction.specular*pow(max(dot(r,v),0),light_direction.shiness);
        }
    }
    for(int i=0;i<4;i++){
        if(!light_point_arr[i].enabled)
            continue;
        light=true;
        vec3 d=normalize(light_point_arr[i].pos-posFrag);
        ambient+=light_point_arr[i].color*light_point_arr[i].ambient;
        diffuse+=light_point_arr[i].color*light_point_arr[i].diffuse*max(dot(d,normalVector),0);
        vec3 r=normalize(reflect(-d,normalVector));
        vec3 v=normalize(camera_pos-posFrag);
        specular+=light_point_arr[i].color*light_point_arr[i].specular*pow(max(dot(r,v),0),light_point_arr[i].shiness);
    }
    if(light)
        FragColor=vec4(ambient+diffuse,1)*color+vec4(specular,1)*spec_color;
    else
        FragColor=color;
//    vec3 objColor=texture(texture_diffuse_0, texCoord);
//    vec3 ambient_color=ambient_light.color*ambient_light.factor_ambient;
//    FragColor = vec4(objColor,1);
    
}
