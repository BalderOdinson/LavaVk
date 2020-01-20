#version 450

#define MAX_FORWARD_LIGHT_COUNT 16

layout(set = 1, binding = 0) uniform sampler2D diffuse;
layout(set = 1, binding = 1) uniform sampler2D emissive;
layout(set = 1, binding = 2) uniform sampler2D ambient;
layout(set = 1, binding = 3) uniform sampler2D specular;
layout(set = 1, binding = 4) uniform sampler2D specular_highlight;

layout(set = 2, binding = 0) uniform sampler2D normal;
layout(set = 2, binding = 1) uniform sampler2D bump;
layout(set = 2, binding = 2) uniform sampler2D displacment;

layout(set = 3, binding = 0) uniform sampler2D alpha;
layout(set = 3, binding = 1) uniform sampler2D reflection;

layout(set = 4, binding = 0) uniform sampler2D metallic;
layout(set = 4, binding = 1) uniform sampler2D roughness;

layout(set = 5, binding = 0) uniform samplerCube cubemap;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in vec3 in_normal;
layout(location = 3) in vec3 in_tangent;
/*layout(location = 4) in vec3 in_bitangent;*/

layout(location = 0) out vec4 o_color;

layout(set = 0, binding = 0) uniform UniformBufferObject
{
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 model_inverse;
    vec3 camera_position;
} ubo;

struct Light
{
    vec4 position;// position.w represents type of light
    vec4 direction;// direction.w represents constant part of intensity
    vec4 info;// info.x represents linear part of intensity, represents quadratic part of intensity, info.z represents light inner cone angle, info.w represents light outer cone angle
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
};

layout(set = 0, binding = 1) uniform LightsInfo
{
    uint count;
    Light light[MAX_FORWARD_LIGHT_COUNT];
} lights;

// Push constants come with a limitation in the size of data.
// The standard requires at least 128 bytes
layout(push_constant, std430) uniform SpecularMaterialUniform
{
    vec4 emissive_color;
    vec4 diffuse_color;
    vec4 ambient_color;
    vec4 specular_color;
    uint flags;
} specular_material;

bool has_alpha() { return (specular_material.flags & (1u << 0u)) > 0; }
bool has_ambient() { return (specular_material.flags & (1u << 1u)) > 0; }
bool has_bump() { return (specular_material.flags & (1u << 2u)) > 0; }
bool has_diffuse() { return (specular_material.flags & (1u << 3u)) > 0; }
bool has_displacment() { return (specular_material.flags & (1u << 4u)) > 0; }
bool has_emissive() { return (specular_material.flags & (1u << 5u)) > 0; }
bool has_metallic() { return (specular_material.flags & (1u << 6u)) > 0; }
bool has_normal() { return (specular_material.flags & (1u << 7u)) > 0; }
bool has_reflection() { return (specular_material.flags & (1u << 8u)) > 0; }
bool has_roughness() { return (specular_material.flags & (1u << 9u)) > 0; }
bool has_specular_highlight() { return (specular_material.flags & (1u << 10u)) > 0; }
bool has_specular() { return (specular_material.flags & (1u << 11u)) > 0; }
bool has_cubemap() { return (specular_material.flags & (1u << 12u)) > 0; }

vec3 cacluclate_directional_light(uint index, vec3 norm, vec3 material_ambient, vec3 material_specular, vec3 material_diffuse)
{
    vec3 light_dir = normalize(-lights.light[index].direction.xyz);
    vec3 light_diffuse = lights.light[index].diffuse.rgb;
    vec3 light_ambient = lights.light[index].ambient.rgb;
    vec3 light_specular = lights.light[index].specular.rgb;

    vec3 view_dir = normalize(ubo.camera_position - in_position);
    vec3 reflect_dir = reflect(-light_dir, norm);

    float shininess = specular_material.specular_color.a;
    if (shininess != 0) shininess = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    vec3 ambient_c = material_ambient * light_ambient;
    vec3 specular_c = material_specular * shininess * light_specular;
    vec3 diffuse_c = max(dot(norm, light_dir), 0.0) * light_diffuse * material_diffuse;

    return ambient_c + specular_c + diffuse_c;
}

vec3 cacluclate_point_light(uint index, vec3 norm, vec3 material_ambient, vec3 material_specular, vec3 material_diffuse)
{
    vec3 light_dir = normalize(lights.light[index].position.xyz - in_position);
    vec3 light_diffuse = lights.light[index].diffuse.rgb;
    vec3 light_ambient = lights.light[index].ambient.rgb;
    vec3 light_specular = lights.light[index].specular.rgb;

    vec3 view_dir = normalize(ubo.camera_position - in_position);
    vec3 reflect_dir = reflect(-light_dir, norm);

    float shininess = specular_material.specular_color.a;
    if (shininess != 0) shininess = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    float distance = length(lights.light[index].position.xyz - in_position);
    float constant = lights.light[index].direction.w;
    float linear = lights.light[index].info.x;
    float quadratic = lights.light[index].info.y;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient_c = material_ambient * light_ambient;
    vec3 specular_c = material_specular * shininess * light_specular;
    vec3 diffuse_c = max(dot(norm, light_dir), 0.0) * light_diffuse * material_diffuse;

    return (ambient_c + specular_c + diffuse_c) * attenuation;
}

vec3 cacluclate_spotlight_light(uint index, vec3 norm, vec3 material_ambient, vec3 material_specular, vec3 material_diffuse)
{
    vec3 light_dir = normalize(lights.light[index].position.xyz - in_position);
    vec3 light_diffuse = lights.light[index].diffuse.rgb;
    vec3 light_ambient = lights.light[index].ambient.rgb;
    vec3 light_specular = lights.light[index].specular.rgb;

    float theta = dot(light_dir, normalize(-lights.light[index].direction.xyz));
    float inner_cutoff = lights.light[index].info.z;
    float outer_cutoff = lights.light[index].info.w;
    float epsilon   = inner_cutoff - outer_cutoff;
    float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);

    vec3 view_dir = normalize(ubo.camera_position - in_position);
    vec3 reflect_dir = reflect(-light_dir, norm);

    float shininess = specular_material.specular_color.a;
    if (shininess != 0) shininess = pow(max(dot(view_dir, reflect_dir), 0.0), shininess);

    float distance = length(lights.light[index].position.xyz - in_position);
    float constant = lights.light[index].direction.w;
    float linear = lights.light[index].info.x;
    float quadratic = lights.light[index].info.y;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient_c = material_ambient * light_ambient;
    vec3 specular_c = material_specular * shininess * light_specular;
    vec3 diffuse_c = max(dot(norm, light_dir), 0.0) * light_diffuse * material_diffuse;

    return (ambient_c + (specular_c + diffuse_c) * intensity) * attenuation;
}

void main(void)
{
    float material_alpha = specular_material.diffuse_color.a;
    if (has_alpha()) material_alpha = texture(alpha, in_uv).r;

    vec3 material_emissive = specular_material.emissive_color.rgb;
    if (has_emissive()) material_emissive *= texture(emissive, in_uv).rgb;

    vec3 material_ambient = specular_material.ambient_color.rgb;
    vec3 material_diffuse = specular_material.diffuse_color.rgb;
    if (has_diffuse())
    {
        material_diffuse *= texture(diffuse, in_uv).rgb;
        material_ambient = material_diffuse;
    }

    if (has_ambient()) material_ambient = specular_material.ambient_color.rgb * texture(ambient, in_uv).rgb;

    vec3 material_specular = specular_material.specular_color.rgb;
    if (has_specular()) material_specular *= texture(specular, in_uv).rgb;
    if (has_specular_highlight()) material_specular *= texture(specular_highlight, in_uv).rgb;

    vec3 norm = in_normal;
    if (has_normal())
    {
        norm = texture(normal, in_uv).rgb;
        // transform normal vector to range [-1,1]
        norm = normalize(norm * 2.0 - 1.0);
    }

    norm = normalize(mat3(ubo.model_inverse) * norm);
    vec3 tangent = normalize(vec3(ubo.model * vec4(in_tangent, 0)));
    tangent = normalize(tangent - dot(tangent, norm) * norm);
    /* vec3 bitangent = cross(norm, tangent);*/

    vec3 result = vec3(0);
    for(uint i = 0; i < lights.count; ++i)
    {
        if (lights.light[i].position.w == 0.f)
        {
            result += cacluclate_directional_light(i, norm, material_ambient, material_specular, material_diffuse);
        }
        else if (lights.light[i].position.w == 1.f)
        {
            result += cacluclate_point_light(i, norm, material_ambient, material_specular, material_diffuse);
        }
        else if (lights.light[i].position.w == 2.f)
        {
            result += cacluclate_spotlight_light(i, norm, material_ambient, material_specular, material_diffuse);
        }
    }

    o_color = vec4(material_emissive + result, material_alpha);
}