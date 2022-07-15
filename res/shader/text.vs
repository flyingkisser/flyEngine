#version 330 core
layout (location = 0) in vec2 aPos; // <vec2 pos, vec2 tex>
layout (location = 1) in vec2 aTexCoord;
out vec2 texCoord;

uniform mat4 projection;

void main()
{
    gl_Position = projection * vec4(aPos.xy, 0.0, 1.0);
    texCoord = aTexCoord;
}
