#version 330 core
precision highp float;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoord;
    mat3 TBN;
    vec3 cam_pos;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture_normal;
uniform bool bFlipX;
uniform bool bFlipY;
uniform bool bReverseColor;
uniform bool bGray;
uniform vec3 lightPos;

void main(){
    float x=fs_in.texCoord.x;
    float y=fs_in.texCoord.y;
    if(bFlipX==true)
        x=1.0f-x;
    if(bFlipY==true)
        y=1.0f-y;
    vec2 pos=vec2(x,y);
    vec4 color=texture(texture0,pos);

    vec3 normal = texture(texture_normal, pos).rgb;
    // FragColor=vec4(vec3(normal),1.0f);
    // return;

    normal = normalize(fs_in.TBN*(normal * 2.0 - 1.0));  //法向量映射到[-1,1]区间，再*上TBN，把法向量从tbn坐标系变换到世界坐标系
   
    vec4 ambient = 0.5 * color;
    vec3 lightDir = normalize(lightPos - fs_in.fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec4 diffuse = diff * color;
    vec3 viewDir = normalize(fs_in.cam_pos - fs_in.fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);

    vec4 specular = vec4(vec3(0.5) * spec,0.0);
    color = ambient + diffuse + specular;

    // if(bReverseColor)
    //     FragColor=vec4(vec3(1.0-color),1.0);  //颜色反转
    // else if(bGray){
    //     float r=0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    //     FragColor=vec4(r,r,r,1.0);
    // }
    // else
        FragColor=vec4(color.rgb,1.0);
}
