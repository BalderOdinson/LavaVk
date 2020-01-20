#version 450
#extension GL_ARB_separate_shader_objects : enable
#define MAX_POINTS 32

layout(lines_adjacency) in;

layout(set = 0, binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 projection;
    mat4 model_inverse;
    vec3 camera_position;
} ubo;

layout(line_strip, max_vertices = MAX_POINTS) out;

layout(location = 0) in vec3 in_color[4];
layout(location = 0) out vec3 o_color;

layout(push_constant) uniform BSplinePushConstant
{
    mat4 segment;
    vec4 color;
    float factor;
} bspline;

void main()
{
    float factor = MAX_POINTS - 1;

    mat4 r = mat4(
    vec4(gl_in[0].gl_Position.x, gl_in[1].gl_Position.x, gl_in[2].gl_Position.x, gl_in[3].gl_Position.x),
    vec4(gl_in[0].gl_Position.y, gl_in[1].gl_Position.y, gl_in[2].gl_Position.y, gl_in[3].gl_Position.y),
    vec4(gl_in[0].gl_Position.z, gl_in[1].gl_Position.z, gl_in[2].gl_Position.z, gl_in[3].gl_Position.z),
    vec4(gl_in[0].gl_Position.w, gl_in[1].gl_Position.w, gl_in[2].gl_Position.w, gl_in[3].gl_Position.w));

    mat4 r_color = mat4(
    vec4(in_color[0].x, in_color[1].x, in_color[2].x, in_color[3].x),
    vec4(in_color[0].y, in_color[1].y, in_color[2].y, in_color[3].y),
    vec4(in_color[0].z, in_color[1].z, in_color[2].z, in_color[3].z),
    vec4(1, 1, 1, 1));

    for (int i = 0; i < MAX_POINTS; ++i)
    {
        float t = i / factor;
        vec4 t_vec = vec4(t*t*t, t*t, t, 1);
        gl_Position = ubo.projection * (t_vec * bspline.factor * bspline.segment * r);
        vec4 color4 = t_vec * bspline.factor * bspline.segment * r_color;
        o_color = color4.rgb;
        EmitVertex();
    }
}
