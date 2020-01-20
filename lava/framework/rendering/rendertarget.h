//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_RENDERTARGET_H
#define LAVAVK_RENDERTARGET_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/imageview.h"
#include "lava/framework/core/device.h"
#include "lava/framework/core/imageobject.h"

namespace LavaVk
{
    /// \brief Description of render pass attachments.
    /// Attachment descriptions can be used to automatically create render target images.
    struct Attachment
    {
        Attachment() = default;
        Attachment(vk::Format format, vk::SampleCountFlagBits samples, const vk::ImageUsageFlags &usage);

        vk::Format format{vk::Format::eUndefined};
        vk::SampleCountFlagBits samples{vk::SampleCountFlagBits::e1};
        vk::ImageUsageFlags usage{vk::ImageUsageFlagBits::eSampled};
    };

    class RenderTarget : public virtual Object
    {
    public:
        explicit RenderTarget(InjectionContext &context);
        RenderTarget(const RenderTarget &) = delete;
        RenderTarget(RenderTarget &&) = delete;

        RenderTarget &operator=(const RenderTarget &) = delete;
        RenderTarget &operator=(RenderTarget &&) = delete;

        const vk::Extent2D &getExtent() const;
        const std::vector<Core::SharedImageView> &getViews() const;
        const std::vector<Attachment> &getAttachments() const;

        /// \brief Sets the current input attachments overwriting the current ones
        /// Should be set before beginning the render pass and before starting a new subpass.
        /// \param input Set of attachment reference number to use as input.
        void setInputAttachments(std::vector<uint32_t> &input);
        const std::vector<uint32_t> &getInputAttachments() const;

        /// \brief Sets the current output attachments overwriting the current ones
        /// Should be set before beginning the render pass and before starting a new subpass.
        /// \param output Set of attachment reference number to use as output.
        void setOutputAttachments(std::vector<uint32_t> &output);
        const std::vector<uint32_t> &getOutputAttachments() const;

        /// \brief Sets the current resolve attachments overwriting the current ones
        /// Should be set before beginning the render pass and before starting a new subpass.
        /// \param resolve Set of attachment reference number to use as referent.
        void setResolveAttachments(std::vector<uint32_t> &resolve);
        const std::vector<uint32_t> &getResolveAttachments() const;

        size_t getHashCode() const override;

        std::type_index getType() const override;

    private:
        Core::SharedDevice device;
        vk::Extent2D extent{};
        std::vector<Core::SharedImageObject> images;
        std::vector<Core::SharedImageView> views;
        std::vector<Attachment> attachments;
        /// By default there are no input attachments.
        std::vector<uint32_t> inputAttachments = {};
        /// By default the output attachments is attachment 0.
        std::vector<uint32_t> outputAttachments = {0};
        /// By default there are no resolve attachments.
        std::vector<uint32_t> resolveAttachments;
    };

    using UniqueRenderTarget = std::unique_ptr<RenderTarget>;
    using SharedRenderTarget = std::shared_ptr<RenderTarget>;
    using WeakRenderTarget = std::weak_ptr<RenderTarget>;
}

#endif //LAVAVK_RENDERTARGET_H
