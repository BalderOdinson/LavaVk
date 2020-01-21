//
// Created by dorian on 16. 12. 2019..
//

#ifndef LAVAVK_SUBPASS_H
#define LAVAVK_SUBPASS_H

#include <unordered_map>
#include "lava/framework/gl_includer.h"
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/commandbuffer.h"
#include "rendercontext.h"
#include "pipelinestate.h"
#include "lava/framework/scene-graph/components/light.h"
#include "lava/framework/core/shadermodule.h"

namespace LavaVk
{
    class Subpass : public virtual Object
    {
    public:
        explicit Subpass(InjectionContext &context);
        Subpass(const Subpass &) = delete;
        Subpass(Subpass &&) = delete;

        Subpass &operator=(const Subpass &) = delete;
        Subpass &operator=(Subpass &&) = delete;

        virtual void prepare(const Core::SharedCommandBuffer &commandBuffer) = 0;

        /// \brief Updates the render target attachments with the ones stored in this subpass
        /// This function is called by the RenderPipeline before beginning the render
        /// pass and before proceeding with a new subpass.
        void updateRenderTargetAttachments();

        /// \brief Draw virtual function
        /// \param commandBuffer Command buffer to use to record draw commands
        virtual void draw(const Core::SharedCommandBuffer &commandBuffer) = 0;

        SharedRenderContext getRenderContext() const;

        DepthStencilState &getDepthStencilState();

        const std::vector<uint32_t> &getInputAttachments() const;
        void setInputAttachments(std::vector<uint32_t> input);

        const std::vector<uint32_t> &getOutputAttachments() const;
        void setOutputAttachments(std::vector<uint32_t> output);

        const std::vector<uint32_t> &getResolveAttachments() const;
        void setResolveAttachments(std::vector<uint32_t> resolve);

    protected:
        SharedResourceCache resourceCache;
        SharedRenderContext renderContext;

    private:
        DepthStencilState depthStencilState{};

        /// Default to no input attachments.
        std::vector<uint32_t> inputAttachments = {};
        /// Default to swapchain output attachment.
        std::vector<uint32_t> outputAttachments = {0};
        /// Default to no resolve attachments.
        std::vector<uint32_t> resolveAttachments = {};
    };

    using UniqueSubpass = std::unique_ptr<Subpass>;
    using SharedSubpass = std::shared_ptr<Subpass>;
    using WeakSubpass = std::weak_ptr<Subpass>;
}


#endif //LAVAVK_SUBPASS_H
