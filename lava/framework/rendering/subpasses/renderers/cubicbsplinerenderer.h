//
// Created by oshikuru on 12. 01. 2020..
//

#ifndef LAVAVK_CUBICBSPLINERENDERER_H
#define LAVAVK_CUBICBSPLINERENDERER_H

#include "pathrenderer.h"

namespace LavaVk
{
    class CubicBSplineVertexShader : public Core::ShaderModuleOptions
    {
    public:
        CubicBSplineVertexShader();
    };

    class CubicBSplineGeometryShader : public Core::ShaderModuleOptions
    {
    public:
        CubicBSplineGeometryShader();
    };

    class CubicBSplineFragmentShader : public Core::ShaderModuleOptions
    {
    public:
        CubicBSplineFragmentShader();
    };

    class CubicBSplineRenderer : public PathRenderer
    {
    public:
        struct alignas(16) BSplinePushConstant
        {
            glm::mat4 segment;
            glm::vec4 color;
            float factor;
        };

        explicit CubicBSplineRenderer(InjectionContext &context);

    protected:
        void updateUniforms(const Core::SharedCommandBuffer &commandBuffer,
                            const std::pair<SharedNode, SharedPathGeometry> &component,
                            const SharedRenderFrame &currentFrame, size_t threadIndex) override;
    public:

        std::type_index getType() const override;
    protected:
        bool shouldRenderPath(const SharedPathGeometry &geometry) override;
    };
}


#endif //LAVAVK_CUBICBSPLINERENDERER_H
