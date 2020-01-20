//
// Created by oshikuru on 12. 01. 2020..
//

#include "cubicbsplinerenderer.h"
#include "lava/framework/scene-graph/components/cubicbspline.h"
#include "meshrenderer.h"

LavaVk::CubicBSplineRenderer::CubicBSplineRenderer(LavaVk::InjectionContext &context) : PathRenderer(context)
{
    shaders[vk::ShaderStageFlagBits::eVertex] = make<CubicBSplineVertexShader>();
    shaders[vk::ShaderStageFlagBits::eGeometry] = make<CubicBSplineGeometryShader>();
    shaders[vk::ShaderStageFlagBits::eFragment] = make<CubicBSplineFragmentShader>();
}

std::type_index LavaVk::CubicBSplineRenderer::getType() const
{
    return typeid(CubicBSplineRenderer());
}

bool LavaVk::CubicBSplineRenderer::shouldRenderPath(const LavaVk::SharedPathGeometry &geometry)
{
    return geometry->getPathType() == typeid(CubicBSpline);
}

void LavaVk::CubicBSplineRenderer::updateUniforms(const LavaVk::Core::SharedCommandBuffer &commandBuffer,
                                                  const std::pair<SharedNode, SharedPathGeometry> &component,
                                                  const LavaVk::SharedRenderFrame &currentFrame, size_t threadIndex)
{
    PathRenderer::updateUniforms(commandBuffer, component, currentFrame, threadIndex);
    auto geometry = std::dynamic_pointer_cast<CubicBSpline>(component.second);
    BSplinePushConstant constant{};
    constant.factor = geometry->factor;
    constant.segment = geometry->periodicSegment;
    constant.color = geometry->lineColor;
    commandBuffer->pushConstants(0, constant);
}

LavaVk::CubicBSplineVertexShader::CubicBSplineVertexShader()
{
    filename = "shaders/bspline.vert.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eVertex;
    resources.push_back(Core::InputShaderResource("position", 3, 1, 1, 0));
    resources.push_back(Core::InputShaderResource("color", 3, 1, 1, 1));
    resources.push_back(
            Core::BufferUniformShaderResource("UniformBufferObject", sizeof(MeshRenderer::Ubo), 1, 0, 0, true));
    resources.push_back(Core::OutputShaderResource("o_color", 3, 1, 1, 0));
}

LavaVk::CubicBSplineGeometryShader::CubicBSplineGeometryShader()
{
    filename = "shaders/bspline.geom.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eGeometry;
    resources.push_back(Core::InputShaderResource("in_color", 3, 1, 1, 1));
    resources.push_back(
            Core::BufferUniformShaderResource("UniformBufferObject", sizeof(MeshRenderer::Ubo), 1, 0, 0, true));
    resources.push_back(
            Core::PushConstantShaderResource("BSplinePushConstant", 0,
                                             sizeof(CubicBSplineRenderer::BSplinePushConstant)));
    resources.push_back(Core::OutputShaderResource("o_color", 3, 1, 1, 0));
}

LavaVk::CubicBSplineFragmentShader::CubicBSplineFragmentShader()
{
    filename = "shaders/bspline.frag.spv";
    entryPoint = "main";
    stage = vk::ShaderStageFlagBits::eFragment;
    resources.push_back(Core::InputShaderResource("in_color", 3, 1, 1, 1));
    resources.push_back(
            Core::PushConstantShaderResource("BSplinePushConstant", 0,
                                             sizeof(CubicBSplineRenderer::BSplinePushConstant)));
    resources.push_back(Core::OutputShaderResource("frag_color", 4, 1, 1, 0));
}
