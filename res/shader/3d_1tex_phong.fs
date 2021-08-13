#version 330 core
in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
out vec4 FragColor;

uniform sampler2D texture1;

uniform vec3  cameras_pos;
uniform vec3  camera_pos1;

uniform vec4  ambient_color;
uniform vec3  ambient_pos;

uniform vec4  material_ambient;
uniform vec4  material_diffuse;
uniform vec4  material_specular;
uniform float material_shininess;

uniform vec4  light_color_0;
uniform vec4  light_color_1;
uniform vec4  light_color_2;
uniform vec4  light_color_3;

uniform vec3 light_pos_0;
uniform vec3 light_pos_1;
uniform vec3 light_pos_2;
uniform vec3 light_pos_3;
uniform vec3 light_pos_4;
uniform vec3 light_pos_5;
uniform vec3 light_pos_6;
uniform vec3 light_pos_7;
uniform vec3 light_pos_8;
uniform vec3 light_pos_9;
uniform vec3 light_pos_10;
uniform vec3 light_pos_11;
uniform vec3 light_pos_12;
uniform vec3 light_pos_13;
uniform vec3 light_pos_14;
uniform vec3 light_pos_15;
uniform vec3 light_pos_16;
uniform vec3 light_pos_17;
uniform vec3 light_pos_18;
uniform vec3 light_pos_19;
uniform vec3 light_pos_20;
uniform vec3 light_pos_21;
uniform vec3 light_pos_22;
uniform vec3 light_pos_23;

void main(){
    vec4 objColor=texture(texture1,texCoord);
    vec4 lightColor=vec4(0,0,0,0);
    bool useLight=false;
    
    //环境光
    if(material_ambient.a!=0 && ambient_color.a!=0){
        lightColor+=ambient_color*material_ambient;
        useLight=true;
    }
    
    //漫反射
    vec3 normal=normalize(normalVector);
    if(material_diffuse.a!=0){
        if(light_color_0.a!=0){
            vec3 lightVector=normalize(light_pos_0-posFrag);
            float diffuse=max(dot(normal,lightVector),0);
            lightColor+=light_color_0*diffuse;
            useLight=true;
        }
        if(light_color_1.a!=0){
            vec3 lightVector=normalize(light_pos_1-posFrag);
            float diffuse=max(dot(normal,lightVector),0);
            lightColor+=light_color_1*diffuse;
            useLight=true;
        }
        
    }
    
    //镜面反射
   if(useLight)
        FragColor=lightColor*objColor;
    else
        FragColor=objColor;
}
