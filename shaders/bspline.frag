#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 in_color;

layout(location = 0) out vec4 frag_color;

layout(push_constant) uniform BSplinePushConstant
{
    mat4 segment;
    vec4 color;
    float factor;
} bspline;

void main()
{
    frag_color = bspline.color * vec4(in_color, 1.0);
}
