#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoord;
layout (location=2) in vec3 aNormal;

out vec2 texCoord;
out vec3 normalVector;
out vec3 posFrag;
out vec3 uni_cam_pos;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

// layout (std140) uniform light_point0{
//     float point0_enabled;   //是否启用
//     vec3 point0_pos;       //位置
//     vec3 point0_color;     //光的颜色

//     vec3 point0_ambient;   //环境光材质系数
//     vec3 point0_diffuse;   //漫反射材质系数
//     vec3 point0_specular;  //镜面反射材质系数
    
//     float point0_constant;     //距离衰减常量
//     float point0_linear;       //距离衰减一次常量系数
//     float point0_quadratic;    //距离衰减二次常量系数
// };

// out VS_OUT{
//     float point0_enabled;   //是否启用
//     vec3 point0_pos;       //位置
//     vec3 point0_color;     //光的颜色

//     vec3 point0_ambient;   //环境光材质系数
//     vec3 point0_diffuse;   //漫反射材质系数
//     vec3 point0_specular;  //镜面反射材质系数
    
//     float point0_constant;     //距离衰减常量
//     float point0_linear;       //距离衰减一次常量系数
//     float point0_quadratic;    //距离衰减二次常量系数
// } vs_out;

uniform mat4 matModel;

void main(){
    gl_Position = proj * view * matModel * vec4(aPos, 1);
    posFrag=vec3(matModel * vec4(aPos, 1));
    texCoord=aTexCoord;
    uni_cam_pos=cam_pos;
    normalVector = mat3(transpose(inverse(matModel))) * aNormal;
    // vs_out.point0_enabled=point0_enabled;
    // vs_out.point0_pos=point0_pos;
    // vs_out.point0_color=point0_color;
    // vs_out.point0_ambient=point0_ambient;
    // vs_out.point0_diffuse=point0_diffuse;
    // vs_out.point0_specular=point0_specular;
    // vs_out.point0_constant=point0_constant;
    // vs_out.point0_linear=point0_linear;
    // vs_out.point0_quadratic=point0_quadratic;
}
