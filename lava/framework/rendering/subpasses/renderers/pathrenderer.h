//
// Created by oshikuru on 11. 01. 2020..
//

#ifndef LAVAVK_PATHRENDERER_H
#define LAVAVK_PATHRENDERER_H

#include "lava/framework/rendering/subpasses/componentrenderer.h"
#include "lava/framework/scene-graph/components/pathgeometry.h"

namespace LavaVk
{
    class PathRenderer : public ComponentRenderer
    {
    public:
        explicit PathRenderer(InjectionContext &context);
        PathRenderer(const PathRenderer &) = delete;
        PathRenderer(PathRenderer&&) = delete;

        PathRenderer &operator=(const PathRenderer &) = delete;
        PathRenderer &operator=(PathRenderer &&) = delete;

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
        virtual std::vector<Core::SharedShaderModule> getShaderModules();
        virtual bool shouldRenderPath(const SharedPathGeometry &geometry) = 0;
        virtual void
        updateUniforms(const Core::SharedCommandBuffer &commandBuffer,
                       const std::pair<SharedNode, SharedPathGeometry> &component, const SharedRenderFrame &currentFrame,
                       size_t threadIndex);

        SharedCamera camera;
        SharedResourceCache resourceCache;
        std::unordered_map<vk::ShaderStageFlagBits, Core::SharedShaderModuleOptions> shaders;

    private:
        AlphaMode nodeMode;

        void drawPathCommand(const Core::SharedCommandBuffer &commandBuffer, const SharedPathGeometry &path);
    };
}


#endif //LAVAVK_PATHRENDERER_H
