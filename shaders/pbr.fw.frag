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
    vec4 color;
};

layout(set = 0, binding = 1) uniform LightsInfo
{
    uint count;
    Light light[MAX_FORWARD_LIGHT_COUNT];
} lights;

// Push constants come with a limitation in the size of data.
// The standard requires at least 128 bytes
layout(push_constant, std430) uniform PBRMaterialUniform
{
    vec4 emissive_color;
    vec4 base_color_factor;
    float metallic_factor;
    float roughness_factor;
    uint flags;
} pbr_material;


bool has_alpha() { return (pbr_material.flags & (1u << 0u)) > 0; }
bool has_ambient() { return (pbr_material.flags & (1u << 1u)) > 0; }
bool has_bump() { return (pbr_material.flags & (1u << 2u)) > 0; }
bool has_diffuse() { return (pbr_material.flags & (1u << 3u)) > 0; }
bool has_displacment() { return (pbr_material.flags & (1u << 4u)) > 0; }
bool has_emissive() { return (pbr_material.flags & (1u << 5u)) > 0; }
bool has_metallic() { return (pbr_material.flags & (1u << 6u)) > 0; }
bool has_normal() { return (pbr_material.flags & (1u << 7u)) > 0; }
bool has_reflection() { return (pbr_material.flags & (1u << 8u)) > 0; }
bool has_roughness() { return (pbr_material.flags & (1u << 9u)) > 0; }
bool has_specular_highlight() { return (pbr_material.flags & (1u << 10u)) > 0; }
bool has_specular() { return (pbr_material.flags & (1u << 11u)) > 0; }
bool has_cubemap() { return (pbr_material.flags & (1u << 12u)) > 0; }

void main(void)
{
    float a = pbr_material.base_color_factor.a;
    if (has_alpha()) a = texture(alpha, in_uv).r;

    vec3 emiss = pbr_material.emissive_color.rgb;
    if (has_emissive()) emiss = texture(emissive, in_uv).rgb;

    vec4 diff = vec4(pbr_material.base_color_factor.rgb, a);
    if (has_diffuse()) diff = texture(diffuse, in_uv);

    vec3 norm = in_normal;
    if (has_normal())
    {
        norm = texture(normal, in_uv).rgb;
        // transform normal vector to range [-1,1]
        norm = normalize(norm * 2.0 - 1.0);
    }

    vec3 light = ubo.camera_position;
    vec3 light_color = vec3(0.7, 0.7, 0.7);
    vec3 light_dir = normalize(light - in_position);

    if(lights.count > 0)
    {
        light = lights.light[0].position.xyz;
        light_color = lights.light[0].color.rgb;
        light_dir = lights.light[0].direction.xyz;
    }

    o_color = vec4(max(dot(norm, light_dir), 0.0) * light_color, 1);
    o_color *= diff;
}
