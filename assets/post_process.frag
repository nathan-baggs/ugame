#version 460 core

in vec3 normal;
in vec2 tex_coord;
in vec4 frag_position;
in mat3 tbn;
out vec4 frag_colour;

uniform sampler2D tex0;
uniform float gamma;

void main()
{
    vec3 hdr_colour = texture(tex0, tex_coord).rgb;

    vec3 mapped = hdr_colour / (hdr_colour + vec3(1.0));

    mapped = pow(mapped, vec3(1.0 / gamma));
    frag_colour = vec4(mapped, 1.0);

}


