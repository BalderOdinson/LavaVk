//
// Created by dorian on 06. 01. 2020..
//

#ifndef LAVAVK_FORWARDLIGHTSRENDERER_H
#define LAVAVK_FORWARDLIGHTSRENDERER_H

#include "lava/framework/rendering/subpasses/componentrenderer.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/scene-graph/components/light.h"
#include "lava/framework/constants.h"

namespace LavaVk
{
    class ForwardLightsRenderer : public ComponentRenderer
    {
    public:
        struct alignas(16) LightUniform
        {
            glm::vec4 position;         // position.w represents type of light
            glm::vec4 direction;        // direction.w represents constant part of intensity
            glm::vec4 info;             // info.x represents linear part of intensity, represents quadratic part of intensity, info.z represents light inner cone angle, info.w represents light outer cone angle
            glm::vec4 color;
        };

        struct alignas(16) ForwardLights
        {
            uint32_t count;
            LightUniform lights[Constants::Light::MaxForwardLightsCount];
        };

        explicit ForwardLightsRenderer(InjectionContext &context);
        ForwardLightsRenderer(const ForwardLightsRenderer &) = delete;
        ForwardLightsRenderer(ForwardLightsRenderer &&) = delete;

        ForwardLightsRenderer &operator=(const ForwardLightsRenderer &) = delete;
        ForwardLightsRenderer &operator=(ForwardLightsRenderer &&) = delete;

        std::type_index getType() const override;
        std::vector<std::pair<SharedNode, SharedComponent>>
        prepare(const SharedScene &scene, const SharedCamera &camera,
                const Core::SharedCommandBuffer &commandBuffer) override;
        void
        draw(const Core::SharedCommandBuffer &commandBuffer, const std::pair<SharedNode, SharedComponent> &component,
             const DepthStencilState &depthStencilState, const std::vector<uint32_t> &inputAttachments,
             const std::vector<uint32_t> &outputAttachments, const std::vector<uint32_t> &resolveAttachments,
             const SharedRenderFrame &currentFrame, size_t threadIndex) override;
        bool tryGetShaderOptions(vk::ShaderStageFlagBits stage, Core::SharedShaderModuleOptions &options) override;

        /// \brief Create a buffer allocation from scene graph lights to be bound to shaders
        /// \tparam T ForwardLights
        /// \param sceneLights  Lights from the scene graph
        /// \param maxLights max lights count
        /// \return BufferAllocation A buffer allocation created for use in shaders
        static BufferAllocation allocateLights(const std::vector<SharedLight> &sceneLights, size_t maxLights,
                                               const SharedRenderFrame &currentFrame,
                                               size_t threadIndex);
    protected:
        std::vector<SharedLight> lights;

    private:
        SharedComponentRenderer baseRenderer;
        uint32_t set;
        uint32_t binding;
    };
}


#endif //LAVAVK_FORWARDLIGHTSRENDERER_H
