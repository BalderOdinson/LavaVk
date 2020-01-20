#version 450
#extension GL_ARB_separate_shader_objects : enable

#define MAX_DEFERRED_LIGHT_COUNT (1000)

layout(input_attachment_index = 0, binding = 0) uniform subpassInput in_depth;
layout(input_attachment_index = 1, binding = 1) uniform subpassInput in_albedo;
layout(input_attachment_index = 2, binding = 2) uniform subpassInput in_specular;
layout(input_attachment_index = 3, binding = 3) uniform subpassInput in_normal;

layout(location = 0) in vec2 in_uv;

layout(location = 0) out vec4 o_color;

layout(set = 0, binding = 3) uniform LightUniform
{
    mat4 inv_view;
    mat4 inv_projection;
    vec2 inv_resolution;
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

layout(set = 0, binding = 4) uniform LightsInfo
{
    uint count;
    Light light[MAX_DEFERRED_LIGHT_COUNT];
} lights;

vec3 cacluclate_directional_light(uint index, vec3 position, vec3 camera_position,  vec3 norm, vec3 material_ambient, vec3 material_specular, vec3 material_diffuse)
{
    vec3 light_dir = normalize(-lights.light[index].direction.xyz);
    vec3 light_diffuse = lights.light[index].diffuse.rgb;
    vec3 light_ambient = lights.light[index].ambient.rgb;
    vec3 light_specular = lights.light[index].specular.rgb;

    vec3 view_dir = normalize(camera_position - position);
    vec3 reflect_dir = reflect(-light_dir, norm);

    vec3 ambient_c = material_ambient * light_ambient;
    vec3 specular_c = material_specular * light_specular;
    vec3 diffuse_c = max(dot(norm, light_dir), 0.0) * light_diffuse * material_diffuse;

    return ambient_c + specular_c + diffuse_c;
}

vec3 cacluclate_point_light(uint index, vec3 position, vec3 camera_position,  vec3 norm, vec3 material_ambient, vec3 material_specular, vec3 material_diffuse)
{
    vec3 light_dir = normalize(lights.light[index].position.xyz - position);
    vec3 light_diffuse = lights.light[index].diffuse.rgb;
    vec3 light_ambient = lights.light[index].ambient.rgb;
    vec3 light_specular = lights.light[index].specular.rgb;

    vec3 view_dir = normalize(camera_position - position);
    vec3 reflect_dir = reflect(-light_dir, norm);

    float distance = length(lights.light[index].position.xyz - position);
    float constant = lights.light[index].direction.w;
    float linear = lights.light[index].info.x;
    float quadratic = lights.light[index].info.y;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient_c = material_ambient * light_ambient;
    vec3 specular_c = material_specular * light_specular;
    vec3 diffuse_c = max(dot(norm, light_dir), 0.0) * light_diffuse * material_diffuse;

    return (ambient_c + specular_c + diffuse_c) * attenuation;
}

vec3 cacluclate_spotlight_light(uint index, vec3 position, vec3 camera_position,  vec3 norm, vec3 material_ambient, vec3 material_specular, vec3 material_diffuse)
{
    vec3 light_dir = normalize(lights.light[index].position.xyz - position);
    vec3 light_diffuse = lights.light[index].diffuse.rgb;
    vec3 light_ambient = lights.light[index].ambient.rgb;
    vec3 light_specular = lights.light[index].specular.rgb;

    float theta = dot(light_dir, normalize(-lights.light[index].direction.xyz));
    float inner_cutoff = lights.light[index].info.z;
    float outer_cutoff = lights.light[index].info.w;
    float epsilon   = inner_cutoff - outer_cutoff;
    float intensity = clamp((theta - outer_cutoff) / epsilon, 0.0, 1.0);

    vec3 view_dir = normalize(camera_position - position);
    vec3 reflect_dir = reflect(-light_dir, norm);

    float distance = length(lights.light[index].position.xyz - position);
    float constant = lights.light[index].direction.w;
    float linear = lights.light[index].info.x;
    float quadratic = lights.light[index].info.y;
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));

    vec3 ambient_c = material_ambient * light_ambient;
    vec3 specular_c = material_specular * light_specular;
    vec3 diffuse_c = max(dot(norm, light_dir), 0.0) * light_diffuse * material_diffuse;

    return (ambient_c + (specular_c + diffuse_c) * intensity) * attenuation;
}

void main()
{
    // Retrieve position from depth
    vec4 clip = vec4(gl_FragCoord.xy * ubo.inv_resolution * 2.0 - 1.0, subpassLoad(in_depth).x, 1.0);
    vec4 world_w = ubo.inv_view * ubo.inv_projection * clip;
    vec3 pos = world_w.xyz / world_w.w;

    vec4 albedo = subpassLoad(in_albedo);
    vec4 specular = subpassLoad(in_specular);
    // Transform from [0,1] to [-1,1]
    vec3 normal = subpassLoad(in_normal).xyz;
    normal = normalize(2.0 * normal - 1.0);

    // Calculate lighting
    vec3 result = vec3(0.0);

    for(uint i = 0; i < lights.count; ++i)
    {
        if (lights.light[i].position.w == 0.f)
        {
            result += cacluclate_directional_light(i, pos, ubo.inv_view[2].xyz, normal, albedo.xyz, specular.xyz, albedo.xyz);
        }
        else if (lights.light[i].position.w == 1.f)
        {
            result += cacluclate_point_light(i, pos, ubo.inv_view[2].xyz, normal, albedo.xyz, specular.xyz, albedo.xyz);
        }
        else if (lights.light[i].position.w == 2.f)
        {
            result += cacluclate_spotlight_light(i, pos, ubo.inv_view[2].xyz, normal, albedo.xyz,specular.xyz, albedo.xyz);
        }
    }

    o_color = vec4(result, 1.0);
}
