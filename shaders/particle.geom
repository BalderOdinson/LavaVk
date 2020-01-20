#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAX_PARTICLE_PROPERTIES_COUNT 10

layout(points) in;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 model_inverse;
    vec3 camera_position;
} ubo;

layout(triangle_strip, max_vertices = 4) out;

layout(location = 0) in vec4 in_color[1];
layout(location = 1) in vec4 in_velocity[1];

layout(location = 0) out vec2 o_texcoord;
layout(location = 1) out vec4 o_color;

layout(push_constant) uniform ParticlePushConstant
{
    float size_multiplier;
    float motion_blur;
    uint stretch_with_velocity;
    float mass[MAX_PARTICLE_PROPERTIES_COUNT];
} particle_info;

void main()
{
    if (in_color[0].w == 0)
    return;

    /*    uint type = uint(gl_in[0].gl_Position.w);
        vec4 position = ubo.view * ubo.model * vec4(gl_in[0].gl_Position.xyz, 1.0);
        float size = particle_info.mass[type] * particle_info.size_multiplier;
        vec3 velocity = mat3(ubo.view) * mat3(ubo.model) * in_velocity[0].xyz;

        vec3 up = vec3(0, 1, 0) * size;
        vec3 right = vec3(1, 0, 0) * size;*/

    uint type = uint(gl_in[0].gl_Position.w);

    vec4 position = ubo.view * ubo.model * vec4(gl_in[0].gl_Position.xyz, 1);
    vec4 vel = ubo.view * ubo.model * vec4(in_velocity[0].xyz, 1);
    vec3 velocity = mat3(ubo.view) * mat3(ubo.model) * in_velocity[0].xyz;

    float size = particle_info.mass[type] * particle_info.size_multiplier;
    /*vec3 velocity = mat3(ubo.view) * mat3(ubo.model) * in_velocity[0].xyz;*/

    vec3 up = vec3(0, size, 0);
    vec3 right = vec3(size, 0, 0);

    vec3 quad_position = - right + up;
    vec3 velocity_effect = vec3(0);
    if(particle_info.stretch_with_velocity > 0)
        velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    /*vec3 velocity_effect = vec3(0);
    if(particle_info.stretch_with_velocity > 0)
        velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    gl_Position = ubo.projection * vec4(quad_position + velocity_effect, position.w);
    o_texcoord = vec2(0.0, 1.0);
    o_color = in_color[0];
    EmitVertex();*/
    gl_Position = ubo.projection * (position + vec4(quad_position + velocity_effect, 0));
    o_texcoord = vec2(0.0, 1.0);
    o_color = in_color[0];
    EmitVertex();

    /*quad_position = position.xyz - right - up;
    velocity_effect = vec3(0);
    if(particle_info.stretch_with_velocity > 0)
        velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    gl_Position = ubo.projection * vec4(quad_position + velocity_effect, position.w);
    o_texcoord = vec2(0.0, 0.0);
    o_color = in_color[0];
    EmitVertex();*/

    quad_position = - right - up;
    if(particle_info.stretch_with_velocity > 0)
        velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    gl_Position = ubo.projection * (position + vec4(quad_position + velocity_effect, 0));
    o_texcoord = vec2(0.0, 0.0);
    o_color = in_color[0];
    EmitVertex();

    /*quad_position = position.xyz + right + up;
    velocity_effect = vec3(0);
    if(particle_info.stretch_with_velocity > 0)
        velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    gl_Position = ubo.projection * vec4(quad_position + velocity_effect, position.w);
    o_texcoord = vec2(1.0, 1.0);
    o_color = in_color[0];
    EmitVertex();*/

    quad_position = right + up;
    if(particle_info.stretch_with_velocity > 0)
        velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    gl_Position = ubo.projection * (position + vec4(quad_position + velocity_effect, 0));
    o_texcoord = vec2(1.0, 1.0);
    o_color = in_color[0];
    EmitVertex();

    /*quad_position = position.xyz + right - up;
    velocity_effect = vec3(0);
    if(particle_info.stretch_with_velocity > 0)
    velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    gl_Position = ubo.projection * vec4(quad_position + velocity_effect, position.w);
    o_texcoord = vec2(1.0, 0.0);
    o_color = in_color[0];
    EmitVertex();*/

    quad_position = right - up;
    if(particle_info.stretch_with_velocity > 0)
        velocity_effect = (dot(normalize(quad_position), normalize(velocity)) * velocity * particle_info.motion_blur);
    gl_Position = ubo.projection * (position + vec4(quad_position + velocity_effect, 0));
    o_texcoord = vec2(1.0, 0.0);
    o_color = in_color[0];
    EmitVertex();

    EndPrimitive();
}
