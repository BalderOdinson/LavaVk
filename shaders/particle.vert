#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec4 velocity;

layout(location = 0) out vec4 o_color;
layout(location = 1) out vec4 o_velocity;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 model_inverse;
    vec3 camera_position;
} ubo;

void main()
{
    gl_Position = position;
    o_color = color;
    o_velocity = velocity;
}
