#version 460 core

out vec4 frag_colour;

in vec3 tex_coord;

uniform samplerCube tex0;

void main()
{
    frag_colour = texture(tex0, tex_coord);
}

