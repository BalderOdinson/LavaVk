#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform sampler2D diffuse;

layout(location = 0) in vec2 in_texcoord;
layout(location = 1) in vec4 in_color;

layout(location = 0) out vec4 frag_color;

void main()
{
    float alpha = 1.0 - dot(2 * in_texcoord - 1, 2 * in_texcoord - 1);
    if (0.2 > alpha) discard;
    frag_color = texture(diffuse, in_texcoord) * in_color;
}
