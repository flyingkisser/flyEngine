#version 330 core
//指定默认的精度限定符
//highp     float(-2^62, 2^62),int(-2^16, 2^16) 32位, 适用于顶点变换
//mediump   float(-2^14, 2^14),int(-2^10, 2^10) 16位, 适用于uv坐标，比highp快2倍
//lowp      float(-2, 2),int(-2^8, 2^8)         10位, 适用于颜色，照明和其他高性能计算，是highp的4倍
precision highp float;

const float gamma = 2.2;

in vec2 texCoord;
in vec3 normalVector;
in vec3 posFrag;
in vec3 uni_cam_pos;
out vec4 FragColor;

#define AREA_LIGHTS_NUM 4
struct AreaLight
{
    bool enabled;
    bool twoSided;
    float intensity;
    float roughness; 
    vec3 pos;
    vec3 color;
    vec3 points0;
    vec3 points1;
    vec3 points2;
    vec3 points3;
};

layout (std140) uniform light_area{
    AreaLight   light_area_arr[AREA_LIGHTS_NUM];
};

// uniform AreaLight areaLight;
uniform sampler2D texture0;
uniform sampler2D texLTC1;
uniform sampler2D texLTC2;

const float LUT_SIZE  = 64.0; // ltc_texture size
const float LUT_SCALE = (LUT_SIZE - 1.0)/LUT_SIZE;
const float LUT_BIAS  = 0.5/LUT_SIZE;

vec3 g_normal_vector;
vec3 g_view_vector;
vec3 g_ambient=vec3(0,0,0);
vec3 g_diffuse=vec3(0,0,0);
vec3 g_specular=vec3(0,0,0);
vec3 g_debug_color=vec3(0,0,0);

// Vector form without project to the plane (dot with the normal)
// Use for proxy sphere clipping
vec3 IntegrateEdgeVec(vec3 v1, vec3 v2){
    // Using built-in acos() function will result flaws
    // Using fitting result for calculating acos()
    float x = dot(v1, v2);
    float y = abs(x);
    float a = 0.8543985 + (0.4965155 + 0.0145206*y)*y;
    float b = 3.4175940 + (4.1616724 + y)*y;
    float v = a / b;
    float theta_sintheta = (x > 0.0) ? v : 0.5*inversesqrt(max(1.0 - x*x, 1e-7)) - v;
    return cross(v1, v2)*theta_sintheta;
}

float IntegrateEdge(vec3 v1, vec3 v2){
    return IntegrateEdgeVec(v1, v2).z;
}

// P is fragPos in world space (LTC distribution)
vec3 LTC_Evaluate(vec3 N, vec3 V, vec3 P, mat3 Minv, vec3 points[4], bool twoSided){
    // construct orthonormal basis around N
    vec3 T1, T2;
    T1 = normalize(V - N * dot(V, N));
    T2 = cross(N, T1);

    // rotate area light in (T1, T2, N) basis
    Minv = Minv * transpose(mat3(T1, T2, N));

    // polygon (allocate 4 vertices for clipping)
    vec3 L[4];
    // transform polygon from LTC back to origin Do (cosine weighted)
    L[0] = Minv * (points[0] - P);
    L[1] = Minv * (points[1] - P);
    L[2] = Minv * (points[2] - P);
    L[3] = Minv * (points[3] - P);

    // use tabulated horizon-clipped sphere
    // check if the shading point is behind the light
    vec3 dir = points[0] - P; // LTC space
    vec3 lightNormal = cross(points[1] - points[0], points[3] - points[0]);
    bool behind = (dot(dir, lightNormal) < 0.0);

    // cos weighted space
    L[0] = normalize(L[0]);
    L[1] = normalize(L[1]);
    L[2] = normalize(L[2]);
    L[3] = normalize(L[3]);

    // integrate
    vec3 vsum = vec3(0.0);
    vsum += IntegrateEdgeVec(L[0], L[1]);
    vsum += IntegrateEdgeVec(L[1], L[2]);
    vsum += IntegrateEdgeVec(L[2], L[3]);
    vsum += IntegrateEdgeVec(L[3], L[0]);

    // form factor of the polygon in direction vsum
    float len = length(vsum);

    float z = vsum.z/len;
    if (behind)
        z = -z;

    vec2 uv = vec2(z*0.5f + 0.5f, len); // range [0, 1]
    uv = uv*LUT_SCALE + LUT_BIAS;

    // Fetch the form factor for horizon clipping
    float scale = texture(texLTC2, uv).w;

    float sum = len*scale;
    if (!behind && !twoSided)
        sum = 0.0;

    // Outgoing radiance (solid angle) for the entire polygon
    vec3 Lo_i = vec3(sum, sum, sum);
    return Lo_i;
}
// PBR-maps for roughness (and metallic) are usually stored in non-linear
// color space (sRGB), so we use these functions to convert into linear RGB.
vec3 PowVec3(vec3 v, float p){
    return vec3(pow(v.x, p), pow(v.y, p), pow(v.z, p));
}
vec3 ToLinear(vec3 v) { return PowVec3(v, gamma); }
vec3 ToSRGB(vec3 v)   { return PowVec3(v, 1.0/gamma); }

vec3 checkArea(int j){
    //  bool twoSided=areaLight.twoSided;
    //  float roughness=areaLight.roughness;
    // float intensity=areaLight.intensity;
    // vec3 pos=areaLight.pos;
    // vec3 color=areaLight.color;
    // vec3 p0=areaLight.points0;
    // vec3 p1=areaLight.points1;
    // vec3 p2=areaLight.points2;
    // vec3 p3=areaLight.points3;

    // int i=0;
    // bool twoSided=light_area_arr[i].twoSided;
    // float roughness=light_area_arr[i].roughness;
    // float intensity=light_area_arr[i].intensity;
    // vec3 pos=light_area_arr[i].pos;
    // vec3 color=light_area_arr[i].color;
    // vec3 p0=light_area_arr[i].points0;
    // vec3 p1=light_area_arr[i].points1;
    // vec3 p2=light_area_arr[i].points2;
    // vec3 p3=light_area_arr[i].points3;

    bool twoSided=light_area_arr[0].twoSided;
    float roughness=light_area_arr[0].roughness;
    float intensity=light_area_arr[0].intensity;
    vec3 pos=light_area_arr[0].pos;
    vec3 color=light_area_arr[0].color;
    vec3 p0=light_area_arr[0].points0;
    vec3 p1=light_area_arr[0].points1;
    vec3 p2=light_area_arr[0].points2;
    vec3 p3=light_area_arr[0].points3;

    vec3 mDiffuse=texture(texture0,texCoord).xyz;
    vec3 mSpecular=ToLinear(vec3(0.23f,0.23f,0.23f));
    float dotNV=clamp(dot(g_normal_vector,g_view_vector),0.0f,1.0f);
    vec2 uv=vec2(roughness,sqrt(1.0f-dotNV));
    uv = uv*LUT_SCALE + LUT_BIAS;
    vec4 t1 = texture(texLTC1, uv);// get 4 parameters for inverse_M
    vec4 t2 = texture(texLTC2, uv);// Get 2 parameters for Fresnel calculation
    mat3 Minv = mat3(
        vec3(t1.x, 0, t1.y),
        vec3(  0,  1,    0),
        vec3(t1.z, 0, t1.w)
    );
    vec3 translatedPoints[4];

    translatedPoints[0] = p0 + pos;
    translatedPoints[1] = p1 + pos;
    translatedPoints[2] = p2 + pos;
    translatedPoints[3] = p3 + pos;


    // translatedPoints[0] = areaLight.points0 + pos;
    // translatedPoints[1] = areaLight.points1 + pos;
    // translatedPoints[2] = areaLight.points2 + pos;
    // translatedPoints[3] = areaLight.points3 + pos;

    // translatedPoints[0] = areaLight.points0 + areaLight.pos;
    // translatedPoints[1] = areaLight.points1 + areaLight.pos;
    // translatedPoints[2] = areaLight.points2 + areaLight.pos;
    // translatedPoints[3] = areaLight.points3 + areaLight.pos;


   // Evaluate LTC shading
    vec3 diffuse=LTC_Evaluate(g_normal_vector,g_view_vector,posFrag,mat3(1),translatedPoints,twoSided);
    vec3 specular=LTC_Evaluate(g_normal_vector,g_view_vector,posFrag,Minv,translatedPoints,twoSided);
    // GGX BRDF shadowing and Fresnel
    // t2.x: shadowedF90 (F90 normally it should be 1.0)
    // t2.y: Smith function for Geometric Attenuation Term, it is dot(V or L, H).
    specular+=mSpecular*t2.x+(1.0f-mSpecular)*t2.y;
    diffuse+=mDiffuse*diffuse;
    return color*intensity*(specular+diffuse);
}

void main(){
    g_normal_vector=normalize(normalVector);
    g_view_vector=normalize(uni_cam_pos-posFrag);
   
    // FragColor = vec4(ToSRGB(checkArea(areaLight)),1.0f);
    // return;

    vec3 result=vec3(0,0,0);
    bool lightDirty=false;
    for(int i=0;i<AREA_LIGHTS_NUM;i++){
        if(light_area_arr[i].enabled){
            lightDirty=true;
            // result+=checkArea(light_area_arr[i]);
            result+=checkArea(i);
        }
    }
    FragColor = vec4(ToSRGB(result),1.0f);
    if(!lightDirty)
        FragColor=vec4(texture(texture0,texCoord).xyz,1.0f);
}

