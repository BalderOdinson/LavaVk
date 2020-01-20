//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_RENDERPASSOPTIONS_H
#define LAVAVK_RENDERPASSOPTIONS_H

#include <vector>
#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/rendering/rendertarget.h"

namespace LavaVk::Core
{
    struct LoadStoreInfo
    {
        vk::AttachmentLoadOp loadOp = vk::AttachmentLoadOp::eClear;
        vk::AttachmentStoreOp storeOp = vk::AttachmentStoreOp::eStore;
    };

    struct SubpassInfo
    {
        std::vector<uint32_t> inputAttachments;
        std::vector<uint32_t> outputAttachments;
        std::vector<uint32_t> resolveAttachments;
    };

    class RenderPassOptions : public virtual Object
    {
    public:
        RenderPassOptions() = default;
        RenderPassOptions(std::vector<Attachment> attachments, std::vector<LoadStoreInfo> loadStoreInfos,
                          std::vector<SubpassInfo> subpasses);

        std::vector<Attachment> attachments;
        std::vector<LoadStoreInfo> loadStoreInfos;
        std::vector<SubpassInfo> subpasses;

        std::type_index getType() const override;
        size_t getHashCode() const override;
    };

    using UniqueRenderPassOptions = std::unique_ptr<RenderPassOptions>;
    using SharedRenderPassOptions = std::shared_ptr<RenderPassOptions>;
    using WeakRenderPassOptions = std::weak_ptr<RenderPassOptions>;
}


#endif //LAVAVK_RENDERPASSOPTIONS_H
