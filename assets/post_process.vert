#version 460 core

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec3 in_tangent;
layout(location = 3) in vec2 in_uv;

out vec2 tex_coord;
out vec4 frag_position;

void main()
{
    gl_Position =  vec4(in_position, 1.0);
    tex_coord = in_uv;
}


