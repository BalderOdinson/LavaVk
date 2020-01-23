//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_SPECULARLIGHTRENDERER_H
#define DEFERRED_SPECULARLIGHTRENDERER_H

#include "basiclightrenderer.h"

namespace LavaVk
{
    class DeferredSpecularVertexShader : public Core::ShaderModuleOptions
    {
    public:
        DeferredSpecularVertexShader();
    };

    class DeferredSpecularFragmentShader : public Core::ShaderModuleOptions
    {
    public:
        DeferredSpecularFragmentShader();
    };

    class SpecularLightRenderer : public BasicLightRenderer
    {
    public:
        struct alignas(16) Light
        {
            glm::vec4 position;         // position.w represents type of light
            glm::vec4 direction;        // direction.w represents constant part of intensity
            glm::vec4 info;             // info.x represents linear part of intensity, represents quadratic part of intensity, info.z represents light inner cone angle, info.w represents light outer cone angle
            glm::vec4 diffuse;
            glm::vec4 ambient;
            glm::vec4 specular;
        };

        struct alignas(16) DeferredLights
        {
            uint32_t count;
            SpecularLightRenderer::Light lights[Constants::Light::MaxDeferredLightsCount];
        };

        explicit SpecularLightRenderer(InjectionContext &context);

        void prepare(const SharedScene &scene, const SharedCamera &camera,
                     const Core::SharedCommandBuffer &commandBuffer) override;

        void draw(const Core::SharedCommandBuffer &commandBuffer, const DepthStencilState &depthStencilState,
                  const std::vector<uint32_t> &inputAttachments, const std::vector<uint32_t> &outputAttachments,
                  const std::vector<uint32_t> &resolveAttachments, const SharedRenderFrame &currentFrame,
                  size_t threadIndex) override;

        std::type_index getType() const override;

    protected:
        BufferAllocation allocateLights(const std::vector<SharedLight> &sceneLights, size_t maxLights,
                                        const SharedRenderFrame &currentFrame, size_t threadIndex) override;
    };
}


#endif //DEFERRED_SPECULARLIGHTRENDERER_H
