//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_RENDERTARGETOPTIONS_H
#define LAVAVK_RENDERTARGETOPTIONS_H

#include <vector>
#include "lava/framework/object.h"
#include "lava/framework/core/imageobject.h"

namespace LavaVk
{
    class RenderTargetOptions : public virtual Object
    {
    public:
        RenderTargetOptions() = default;
        explicit RenderTargetOptions(std::vector<Core::SharedImageObject> images);

        void create(const Core::SharedImageObject& swapchainImage);
        void recreate(const Core::SharedImageObject& swapchainImage);

        std::vector<Core::SharedImageObject> images;
        /// Initial values for input attachments. Can be set later.
        std::vector<uint32_t> inputAttachments;
        /// Initial values for output attachments. Can be set later.
        std::vector<uint32_t> outputAttachments;
        /// Initial values for resolve attachments.  Can be set later.
        std::vector<uint32_t> resolveAttachments;

        std::type_index getType() const override;

    protected:
        virtual void create(vk::Extent3D swapchainExtent, vk::SampleCountFlagBits samples) = 0;
    };

    using UniqueRenderTargetOptions = std::unique_ptr<RenderTargetOptions>;
    using SharedRenderTargetOptions = std::shared_ptr<RenderTargetOptions>;
    using WeakRenderTargetOptions = std::weak_ptr<RenderTargetOptions>;

    class DefaultRenderTargetOptions : public RenderTargetOptions
    {
    public:
        DefaultRenderTargetOptions() = default;

        std::type_index getType() const override;

    protected:
        void create(vk::Extent3D swapchainExtent, vk::SampleCountFlagBits samples) override;
    };

    class DeferredRenderTargetOptions : public RenderTargetOptions
    {
    public:
        DeferredRenderTargetOptions() = default;

        std::type_index getType() const override;

    protected:
        void create(vk::Extent3D swapchainExtent, vk::SampleCountFlagBits samples) override;
    };

    class DeferredSpecularRenderTargetOptions : public RenderTargetOptions
    {
    public:
        DeferredSpecularRenderTargetOptions() = default;

        std::type_index getType() const override;

    protected:
        void create(vk::Extent3D swapchainExtent, vk::SampleCountFlagBits samples) override;
    };
}


#endif //LAVAVK_RENDERTARGETOPTIONS_H
