#version 330 core
precision highp float;

in vec2 texCoord;
in vec3 view_pos;

out float FragColor;

uniform sampler2D texture0;

void main()
{            
     float r=0.0; 
     vec2 texSize = 1.0/vec2(textureSize(texture0,0));
     for(int i=-2;i<2;i++){
        for(int j=-2;j<2;j++){
            vec2 offset=vec2(i,j)*texSize;
            r+=texture(texture0,texCoord+offset).r;
        }
     }
     FragColor=r/16;
}