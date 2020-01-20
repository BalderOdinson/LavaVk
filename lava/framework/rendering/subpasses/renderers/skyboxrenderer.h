//
// Created by dorian on 08. 01. 2020..
//

#ifndef LAVAVK_SKYBOXRENDERER_H
#define LAVAVK_SKYBOXRENDERER_H

#include "lava/framework/rendering/subpasses/componentrenderer.h"
#include "lava/framework/scene-graph/components/skybox.h"
#include "lava/framework/dependency-injection/dicontainer.h"

namespace LavaVk
{
    class SkyboxVertexShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        SkyboxVertexShaderOptions();
    };

    class SkyboxFragmentShaderOptions : public Core::ShaderModuleOptions
    {
    public:
        SkyboxFragmentShaderOptions();
    };

    class SkyboxRenderer : public ComponentRenderer
    {
    public:
        /// \brief Global uniform structure for skybox shader
        struct alignas(16) Ubo
        {
            glm::mat4 view;
            glm::mat4 projection;
        };

        explicit SkyboxRenderer(InjectionContext &context);

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

    protected:
        virtual void
        updateUniforms(const Core::SharedCommandBuffer &commandBuffer,
                       const SharedRenderFrame &currentFrame,
                       size_t threadIndex);
        virtual size_t getSetCount() const;
        virtual std::vector<Core::SharedShaderModule> getShaderModules();

        std::unordered_map<vk::ShaderStageFlagBits, Core::SharedShaderModuleOptions> shaders;

    private:
        SharedSkybox skybox;
        SharedCamera camera;
        SharedResourceCache resourceCache;

        void drawGeometryCommand(const Core::SharedCommandBuffer &commandBuffer, const SharedGeometry &geometry);
    };
}


#endif //LAVAVK_SKYBOXRENDERER_H
