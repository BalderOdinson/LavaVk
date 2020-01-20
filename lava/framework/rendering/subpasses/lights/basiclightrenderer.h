//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_BASICLIGHTRENDERER_H
#define DEFERRED_BASICLIGHTRENDERER_H

#include "lava/framework/rendering/subpasses/lightrenderer.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/constants.h"

namespace LavaVk
{
    class DeferredVertexShader : public Core::ShaderModuleOptions
    {
    public:
        DeferredVertexShader();
    };

    class DeferredFragmentShader : public Core::ShaderModuleOptions
    {
    public:
        DeferredFragmentShader();
    };

    class BasicLightRenderer : public LightRenderer
    {
    public:
        /// @brief Light uniform structure for lighting shader
        /// Inverse view projection matrix and inverse resolution vector are used
        /// in lighting pass to reconstruct position from depth and frag coord
        struct alignas(16) LightUniform
        {
            glm::mat4 invView;
            glm::mat4 invProjection;
            glm::vec2 invResolution;
        };

        struct alignas(16) Light
        {
            glm::vec4 position;         // position.w represents type of light
            glm::vec4 direction;        // direction.w represents constant part of intensity
            glm::vec4 info;             // info.x represents linear part of intensity, represents quadratic part of intensity, info.z represents light inner cone angle, info.w represents light outer cone angle
            glm::vec4 color;
        };

        struct alignas(16) DeferredLights
        {
            uint32_t count;
            BasicLightRenderer::Light lights[Constants::Light::MaxDeferredLightsCount];
        };

        explicit BasicLightRenderer(InjectionContext &context);

        void prepare(const SharedScene &scene, const SharedCamera &camera,
                     const Core::SharedCommandBuffer &commandBuffer) override;
        void draw(const Core::SharedCommandBuffer &commandBuffer, const DepthStencilState &depthStencilState,
                  const std::vector<uint32_t> &inputAttachments, const std::vector<uint32_t> &outputAttachments,
                  const std::vector<uint32_t> &resolveAttachments, const SharedRenderFrame &currentFrame,
                  size_t threadIndex) override;

        std::vector<Core::SharedShaderModule> getShaderModules() override;

        std::type_index getType() const override;

    protected:
        virtual BufferAllocation allocateLights(const std::vector<SharedLight> &sceneLights, size_t maxLights,
                                                const SharedRenderFrame &currentFrame, size_t threadIndex);

        std::unordered_map<vk::ShaderStageFlagBits, Core::SharedShaderModuleOptions> shaders;

        SharedScene scene;
        SharedCamera camera;
        SharedResourceCache resourceCache;
        std::vector<SharedLight> sceneLights;
    };
}


#endif //DEFERRED_BASICLIGHTRENDERER_H
