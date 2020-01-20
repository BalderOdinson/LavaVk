//
// Created by dorian on 16. 12. 2019..
//

#ifndef LAVAVK_RENDERPIPELINE_H
#define LAVAVK_RENDERPIPELINE_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include <lava/framework/rendering/options/renderpipelineoptions.h>
#include "lava/framework/object.h"
#include "subpass.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/commandbuffer.h"
#include "lava/framework/core/options/renderpassoptions.h"
#include "renderframe.h"

namespace LavaVk
{
    class RenderPipeline : public virtual Object
    {
    public:
        explicit RenderPipeline(InjectionContext &context);
        explicit RenderPipeline(const RenderPipelineOptions &options);
        RenderPipeline(const RenderPipeline &) = delete;
        RenderPipeline(RenderPipeline &&) = delete;

        RenderPipeline &operator=(const RenderPipeline &) = delete;
        RenderPipeline &operator=(RenderPipeline &&) = delete;

        void prepare(const Core::SharedCommandBuffer &commandBuffer);

        /// \return Load store info.
        const std::vector<Core::LoadStoreInfo> &getLoadStore() const;

        /// \param load_store Load store info to set.
        void setLoadStore(const std::vector<Core::LoadStoreInfo> &loadStore);

        /// \return Clear values.
        const std::vector<vk::ClearValue> &getClearValue() const;

        /// \param clear_values Clear values to set.
        void setClearValue(const std::vector<vk::ClearValue> &clearValues);

        /// \brief Appends a subpass to the pipeline.
        /// \param subpass Subpass to append.
        void addSubpass(SharedSubpass subpass);

        std::vector<SharedSubpass> &getSubpasses();

        /// \brief Record draw commands for each Subpass.
        Core::BeginRenderPassToken
        draw(const Core::SharedCommandBuffer &commandBuffer, const SharedRenderTarget &renderTarget,
             vk::SubpassContents contents = vk::SubpassContents::eInline);

        /// \return Subpass currently being recorded, or the first one
        /// if drawing has not started.
        SharedSubpass getActiveSubpass();

        std::type_index getType() const override;

    private:
        std::vector<SharedSubpass> subpasses;

        /// Default to two load store
        std::vector<Core::LoadStoreInfo> loadStore = std::vector<Core::LoadStoreInfo>(2);

        /// Default to two clear values
        std::vector<vk::ClearValue> clearValue = std::vector<vk::ClearValue>(2);

        size_t activeSubpassIndex{0};
    };

    using UniqueRenderPipeline = std::unique_ptr<RenderPipeline>;
    using SharedRenderPipeline = std::shared_ptr<RenderPipeline>;
    using WeakRenderPipeline = std::weak_ptr<RenderPipeline>;
}


#endif //LAVAVK_RENDERPIPELINE_H
