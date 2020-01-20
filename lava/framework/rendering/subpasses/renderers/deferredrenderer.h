//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_DEFERREDRENDERER_H
#define DEFERRED_DEFERREDRENDERER_H

#include "lava/framework/rendering/subpasses/componentrenderer.h"
#include "lava/framework/dependency-injection/dicontainer.h"

namespace LavaVk
{
    class DeferredRenderer : public ComponentRenderer
    {
    public:
        explicit DeferredRenderer(InjectionContext &context);

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
        SharedComponentRenderer baseRenderer;
    };
}


#endif //DEFERRED_DEFERREDRENDERER_H
