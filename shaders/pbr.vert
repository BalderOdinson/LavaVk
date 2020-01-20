#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 tangent;
//layout(location = 4) in vec3 bitangent;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 model_inverse;
    vec3 camera_position;
} ubo;

layout(location = 0) out vec3 o_position;
layout (location = 1) out vec2 o_uv;
layout(location = 2) out vec3 o_normal;
layout(location = 3) out vec3 o_tangent;
//layout(location = 4) out vec3 o_bitangent;

void main(void)
{
    o_uv = texcoord;
    o_position = vec3(ubo.model * vec4(position, 0));
    o_normal = normalize(vec3(ubo.model * vec4(normal, 0)));
    o_tangent = normalize(vec3(ubo.model * vec4(tangent, 0)));
    o_tangent = normalize(o_tangent - dot(o_tangent, o_normal) * o_normal);
    /*o_bitangent = cross(o_normal, o_tangent);*/
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(position, 1.0);
}