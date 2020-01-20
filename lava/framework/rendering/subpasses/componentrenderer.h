//
// Created by dorian on 05. 01. 2020..
//

#ifndef LAVAVK_COMPONENTRENDERER_H
#define LAVAVK_COMPONENTRENDERER_H

#include <vector>
#include "lava/framework/scene-graph/components/camera.h"
#include "lava/framework/object.h"
#include "lava/framework/core/commandbuffer.h"
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/rendering/renderframe.h"
#include "lava/framework/core/options/shadermoduleoptions.h"

namespace LavaVk
{
    class ComponentRenderer : public virtual Object
    {
    public:
        virtual std::vector<std::pair<SharedNode, SharedComponent>>
        prepare(const SharedScene &scene, const SharedCamera &camera,
                const Core::SharedCommandBuffer &commandBuffer) = 0;
        virtual void
        draw(const Core::SharedCommandBuffer &commandBuffer, const std::pair<SharedNode, SharedComponent> &component,
             const DepthStencilState &depthStencilState, const std::vector<uint32_t> &inputAttachments,
             const std::vector<uint32_t> &outputAttachments, const std::vector<uint32_t> &resolveAttachments,
             const SharedRenderFrame &currentFrame, size_t threadIndex) = 0;

        virtual bool tryGetShaderOptions(vk::ShaderStageFlagBits stage, Core::SharedShaderModuleOptions& options) = 0;
    };

    using UniqueComponentRenderer = std::unique_ptr<ComponentRenderer>;
    using SharedComponentRenderer = std::shared_ptr<ComponentRenderer>;
    using WeakComponentRenderer = std::weak_ptr<ComponentRenderer>;
}


#endif //LAVAVK_COMPONENTRENDERER_H
