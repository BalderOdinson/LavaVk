//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_RENDERPASS_H
#define LAVAVK_RENDERPASS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/device.h"

namespace LavaVk::Core
{
    class RenderPass : public virtual Object
    {
    public:
        explicit RenderPass(InjectionContext &context);
        RenderPass(const RenderPass &) = delete;
        RenderPass(RenderPass &&) = delete;
        ~RenderPass() override;

        RenderPass &operator=(const RenderPass &) = delete;
        RenderPass &operator=(RenderPass &&) = delete;

        vk::RenderPass getHandle() const;
        uint32_t getColorOutputCount(uint32_t subpassIndex) const;

        std::type_index getType() const override;

    private:
        SharedDevice device;
        vk::RenderPass handle;
        size_t subpassCount;

        // Store attachments for every subpass
        std::vector<std::vector<vk::AttachmentReference>> inputAttachments;
        std::vector<std::vector<vk::AttachmentReference>> colorAttachments;
        std::vector<std::vector<vk::AttachmentReference>> depthStencilAttachments;
        std::vector<std::vector<vk::AttachmentReference>> resolveAttachments;
    };

    using UniqueRenderPass = std::unique_ptr<RenderPass>;
    using SharedRenderPass = std::shared_ptr<RenderPass>;
    using WeakRenderPass = std::weak_ptr<RenderPass>;
}



#endif //LAVAVK_RENDERPASS_H
