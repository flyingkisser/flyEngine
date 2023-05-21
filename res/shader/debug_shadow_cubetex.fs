#version 330 core
//指定默认的精度限定符
//highp     float(-2^62, 2^62),int(-2^16, 2^16) 32位, 适用于顶点变换
//mediump   float(-2^14, 2^14),int(-2^10, 2^10) 16位, 适用于uv坐标，比highp快2倍
//lowp      float(-2, 2),int(-2^8, 2^8)         10位, 适用于颜色，照明和其他高性能计算，是highp的4倍
precision highp float;


in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;

out vec4 FragColor;


uniform float far_plane;
uniform vec3 lightPos;
uniform sampler2D texture0;
uniform samplerCube texture_depth_cube;

vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);
vec3 g_debug_color=vec3(0,0,0);


float isInShadowByCube(vec3 fragPos,vec3 lightPos){
    vec3 light2frag = fragPos-lightPos;
    float depthValue=texture(texture_depth_cube,light2frag).r*far_plane;
    return length(light2frag)-0.008 > depthValue ? 1.0 : 0.0;
}

void main(){
    g_normal_vector=normalize(normalVector);
    g_view_vector=normalize(uni_cam_pos-posFrag);
    vec4 obj_color=texture(texture0,texCoord);
    float shadowValue=isInShadowByCube(posFrag,lightPos);
    FragColor=(1.0-shadowValue)*obj_color;
}
