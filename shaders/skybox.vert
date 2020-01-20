#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;
layout(location = 0) out vec3 o_texcoord;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 view;
    mat4 projection;
} ubo;

void main()
{
    gl_Position = (ubo.projection * ubo.view * vec4(position, 1.0)).xyww;
    o_texcoord = position;
}
