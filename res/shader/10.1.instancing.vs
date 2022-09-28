#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;
layout (location = 3) in mat4 aMatModel; 

// layout (location = 3) in vec4 aMatModelCol0;
// layout (location = 4) in vec4 aMatModelCol1;
// layout (location = 5) in vec4 aMatModelCol2; 
// layout (location = 6) in vec4 aMatModelCol3; 


out vec3 fColor;

void main()
{
    fColor = aColor;
    gl_Position = aMatModel*vec4(aPos + aOffset, 0.0, 1.0);

     // mat4 matModel=mat4(aMatModelCol0,aMatModelCol1,aMatModelCol2,aMatModelCol3);
    // vec4 aMatModelCol0=aMatModel[0];
    // vec4 aMatModelCol1=aMatModel[1];
    // vec4 aMatModelCol2=aMatModel[2];
    // vec4 aMatModelCol3=aMatModel[3];

    // fColor.r=aMatModelCol3[3];
    // fColor.g=aMatModelCol3[1];
    // fColor.b=aMatModelCol3[2];
    // fColor.r=aMatModelCol2[2][0];
    // fColor.g=aMatModelCol2[2][1];
    // fColor.b=aMatModelCol2[2][2];
}
