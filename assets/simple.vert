#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_uv;

out vec3 normal;
out vec2 tex_coord;
out vec4 frag_position;
out mat3 tbn;

uniform mat4 model;

layout(std140, binding = 0) uniform camera
{
    mat4 view;
    mat4 projection;
    vec3 eye;
};

void main()
{
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    normal = normalize(transpose(inverse(mat3(model))) * in_normal);
    tex_coord = in_uv;

    vec3 t = normalize(vec3(model * vec4(in_tangent, 0.0)));
    vec3 n = normalize(vec3(model * vec4(in_normal, 0.0)));
    vec3 b = cross(n, t);
    tbn = mat3(t, b, n);

    frag_position = model * vec4(in_position, 1);
}

