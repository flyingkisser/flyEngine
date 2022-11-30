#version 330 core
precision highp float;

in VS_OUT {
    vec3 fragPos;
    vec2 texCoord;
    mat3 TBN;
    mat3 TBNReverse;
    vec3 cam_pos;
} fs_in;

out vec4 FragColor;

uniform sampler2D texture0;
uniform sampler2D texture_normal;
uniform sampler2D texture_height;
uniform bool bFlipX;
uniform bool bFlipY;
uniform bool bReverseColor;
uniform bool bGray;
uniform bool bParallax;
uniform vec3 lightPos;

//视差映射
vec2 calcParallaxMap(vec2 texCoord,vec3 viewDir){
    float height_scale=0.1f;
    float height=texture(texture_height,texCoord).r;
    vec2 offset=viewDir.xy/viewDir.z*(height*height_scale);
    return texCoord-offset;
}

//陡峭视差映射
vec2 calcSteepParallaxMap(vec2 texCoord,vec3 viewDir){
    float height_scale=0.1f;
    float numLayers = mix(32, 8, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    float depth=0.0f;
    float height=texture(texture_height,texCoord).r;
    vec2 texInner=viewDir.xy/viewDir.z*height_scale/numLayers;
    vec2 curTexCoord=texCoord;
    while(depth<height){
        curTexCoord-=texInner;
        height=texture(texture_height,curTexCoord).r;
        depth+=1.0/numLayers;
    }
    return curTexCoord;
}

//遮蔽视差映射
vec2 calcOcclusionParallaxMap(vec2 texCoord,vec3 viewDir){
    float height_scale=0.1f;
    float numLayers = mix(32, 8, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    float depth=0.0f;
    float height=texture(texture_height,texCoord).r;
    vec2 texInner=viewDir.xy/viewDir.z*height_scale/numLayers;
    vec2 curTexCoord=texCoord;
    while(depth<height){
        curTexCoord-=texInner;
        height=texture(texture_height,curTexCoord).r;
        depth+=1.0/numLayers;
    }
    vec2 preTexCoord=curTexCoord+texInner;
    //片元上方最后一个点的采样深度与层级深度的差值
    float beforeDepth=texture(texture_height,preTexCoord).r-depth+1.0/numLayers;
    float afterDepth=height-depth;  //片元下方这个点的采样深度与层级深度的差值
    float weight=afterDepth/(afterDepth-beforeDepth);
    vec2 finalTexCoord=preTexCoord*weight+curTexCoord*(1.0-weight);
    return finalTexCoord;
}

void main(){
    float x=fs_in.texCoord.x;
    float y=fs_in.texCoord.y;
    if(bFlipX==true)
        x=1.0f-x;
    if(bFlipY==true)
        y=1.0f-y;
    vec2 pos=vec2(x,y);

    if(bParallax){
        pos=calcOcclusionParallaxMap(fs_in.texCoord,
            fs_in.TBNReverse*normalize(fs_in.cam_pos-fs_in.fragPos));
        if(pos.x > 1.0 || pos.y > 1.0 || pos.x < 0.0 || pos.y < 0.0)
            discard;
    }

    vec4 color=texture(texture0,pos);
    vec3 normal = texture(texture_normal, pos).rgb;
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
