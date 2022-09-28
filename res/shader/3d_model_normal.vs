#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out VS_OUT {
    vec3 normal;
} vs_out;

layout (std140) uniform mat3d{
    mat4 proj;
    mat4 view;
    vec3 cam_pos;
};

uniform mat4 matModel;

void main()
{
    // vs_out.texCoords = aTexCoords;
    // gl_Position = projection * view * model * vec4(aPos, 1.0); 
    gl_Position = proj * view * matModel * vec4(aPos, 1);
    mat3 normalMatrix = mat3(transpose(inverse(view * matModel)));
    vs_out.normal = normalize(vec3(proj*vec4(normalMatrix * aNormal, 0.0)));
}