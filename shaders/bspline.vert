#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;

layout(location = 0) out vec3 o_color;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 model_inverse;
    vec3 camera_position;
} ubo;

void main()
{
    gl_Position = ubo.view * ubo.model * vec4(position, 1.0);
    o_color = color;
}
