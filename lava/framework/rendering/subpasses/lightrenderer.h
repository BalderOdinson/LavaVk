//
// Created by oshikuru on 20. 01. 2020..
//

#ifndef DEFERRED_LIGHTRENDERER_H
#define DEFERRED_LIGHTRENDERER_H

#include <vector>
#include "lava/framework/rendering/renderframe.h"
#include "lava/scene-graph.h"
#include "lava/framework/bufferpool.h"

namespace LavaVk
{
    class LightRenderer : public virtual Object
    {
    public:
        virtual void prepare(const SharedScene &scene, const SharedCamera &camera,
                             const Core::SharedCommandBuffer &commandBuffer) = 0;
        virtual void draw(const Core::SharedCommandBuffer &commandBuffer, const DepthStencilState &depthStencilState,
                          const std::vector<uint32_t> &inputAttachments, const std::vector<uint32_t> &outputAttachments,
                          const std::vector<uint32_t> &resolveAttachments, const SharedRenderFrame &currentFrame,
                          size_t threadIndex) = 0;

        virtual std::vector<Core::SharedShaderModule> getShaderModules() = 0;
    };

    using UniqueLightRenderer = std::unique_ptr<LightRenderer>;
    using SharedLightRenderer = std::shared_ptr<LightRenderer>;
    using WeakLightRenderer = std::weak_ptr<LightRenderer>;
}


#endif //DEFERRED_LIGHTRENDERER_H
