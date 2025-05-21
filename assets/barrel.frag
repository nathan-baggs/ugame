#version 460 core

in vec3 normal;
in vec2 tex_coord;
in vec4 frag_position;
in mat3 tbn;
out vec4 frag_colour;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform vec3 tint_colour;
uniform float tint_amount;

layout(std140, binding = 0) uniform camera
{
    mat4 view;
    mat4 projection;
    vec3 eye;
};

struct PointLight
{
    vec3 point;
    vec3 point_colour;
    vec3 attenuation;
};

layout(std430, binding = 1) readonly buffer lights
{
    vec3 ambient;
    vec3 direction;
    vec3 direction_colour;
    int num_points;
    PointLight points[];
};

vec3 calc_ambient()
{
    return ambient;
}

vec3 calc_direction()
{
    vec3 light_dir = normalize(-direction);
    float diff = max(dot(normal, light_dir), 0.0);
    return diff * direction_colour;
}

vec3 calc_point(int index)
{
    vec3 point = points[index].point;
    vec3 point_colour = points[index].point_colour;
    vec3 attenuation = points[index].attenuation;

    vec3 n = texture(tex2, tex_coord).xyz;
    n = (n * 2.0) - 1.0;
    n = normalize(tbn * n);

    float distance = length(point - frag_position.xyz);
    float att = 1.0 / (attenuation.x + (attenuation.y * distance) + (attenuation.z * (distance * distance)));

    vec3 light_dir = normalize(point - frag_position.xyz);
    float diff = max(dot(n, light_dir), 0.0);

    vec3 reflect_dir = reflect(-light_dir, n);
    float spec = pow(max(dot(normalize(eye - frag_position.xyz), reflect_dir), 0.0), 32) * texture(tex1, tex_coord).r;

    return ((diff + spec) * att) * point_colour;
}

void main()
{
    vec4 albedo = texture(tex0, tex_coord);
    vec3 colour = calc_ambient();
    colour += calc_direction();

    for (int i = 0; i < num_points; ++i)
    {
        colour += calc_point(i);
    }

    frag_colour = vec4(mix(colour * albedo.rgb, tint_colour, tint_amount), 1.0);
}

