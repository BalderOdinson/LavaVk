//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_IMAGE_H
#define LAVAVK_IMAGE_H

#include <vulkan/vulkan.hpp>
#include "lava/third-party/vma.h"
#include "allocation.h"
#include "lava/framework/event-system/event.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "options/imageoptions.h"
#include "imageobject.h"

namespace LavaVk::Core
{
    class Image : public Allocation, public ImageObject
    {
    public:
        explicit Image(InjectionContext &context);
        explicit Image(ImageOptions options);
        Image(const Image &) = delete;
        Image(Image &&) = delete;
        ~Image() override;

        Image &operator=(const Image &) = delete;
        Image &operator=(Image &&) = delete;

        vk::Image getHandle() const override;

        Event<EventHandler<>> &rebonded() override;
        Event<EventHandler<>> &recreated() override;

        vk::ImageType getImageType() const override;
        const vk::Extent3D &getExtent() const override;
        vk::Format getFormat() const override;
        vk::SampleCountFlagBits getSampleCount() const override;
        vk::ImageUsageFlags getUsage() const override;
        vk::ImageTiling getTiling() const override;
        vk::ImageSubresource getSubresource() const override;

        vma::Allocation getAllocation() const override;
        SharedAllocator getAllocator() const override;
        void recreate() override;
        void rebind() override;
        std::type_index getType() const override;

        SharedDevice getDevice() const override;

    protected:
        void getData(void *data, size_t *size) const override;
        SharedImageUpdater getUpdater() override;

    private:
        SharedAllocator allocator;
        vk::Image handle;
        vma::Allocation allocationHandle;
        ImageOptions options;
        EventOwner<EventHandler<>> rebondedEventSubject;
        EventOwner<EventHandler<>> recreatedChangedEventSubject;

        static vk::ImageType findImageType(vk::Extent3D extent);
    };

    using UniqueImage = std::unique_ptr<Image>;
    using SharedImage = std::shared_ptr<Image>;
    using WeakImage = std::weak_ptr<Image>;
}


#endif //LAVAVK_IMAGE_H
