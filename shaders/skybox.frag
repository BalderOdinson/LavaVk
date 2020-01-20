#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set = 0, binding = 1) uniform samplerCube cubemap;

layout(location = 0) in vec3 in_texcoord;

layout(location = 0) out vec4 frag_color;

void main()
{
    frag_color = texture(cubemap, in_texcoord);
}
