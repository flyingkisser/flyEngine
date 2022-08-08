#version 330 core
in vec2 texCoord;
out vec4 FragColor;

uniform sampler2D texture0;
uniform bool bFlipX;
uniform bool bFlipY;
uniform bool bReverseColor;
uniform bool bGray;

const float offset = 1.0 / 300.0;
void main(){
    float x=texCoord.x;
    float y=texCoord.y;
    if(bFlipX==true)
        x=1-texCoord.x;
    if(bFlipY==true)
        y=1-texCoord.y;
    vec2 pos=vec2(x,y);
    
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right
    );

//    float kernel[9] = float[](
//        -1, -1, -1,
//        -1,  9, -1,
//        -1, -1, -1
//    );
    
    //edge
    float kernel[9] = float[](
        1, 1, 1,
        1, -8, 1,
        1, 1, 1
    );
    
    //blur kernel
//    float kernel[9] = float[](
//        1.0 / 16, 2.0 / 16, 1.0 / 16,
//        2.0 / 16, 4.0 / 16, 2.0 / 16,
//        1.0 / 16, 2.0 / 16, 1.0 / 16
//    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++){
        vec3 color=vec3(texture(texture0, pos.xy + offsets[i]));
        if(bReverseColor)
            sampleTex[i] = vec3(1.0-color);
        else if(bGray){
            float r=0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
            sampleTex[i]=vec3(r,r,r);
        }else
            sampleTex[i] = color;
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];
    FragColor = vec4(col, 1.0);
}
