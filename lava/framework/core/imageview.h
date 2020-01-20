//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_IMAGEVIEW_H
#define LAVAVK_IMAGEVIEW_H

#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "imageobject.h"
#include "device.h"
#include "lava/framework/event-system/event.h"
#include "options/imageviewoptions.h"

namespace LavaVk::Core
{
    class ImageView : public virtual Object
    {
    public:
        explicit ImageView(InjectionContext &context);
        ImageView(ImageViewOptions options);
        ImageView(const ImageView &) = delete;
        ImageView(ImageView &&) = delete;
        ~ImageView() override;

        ImageView &operator=(const ImageView &) = delete;
        ImageView &operator=(ImageView &&) = delete;

        vk::ImageView getHandle() const;

        SharedImageObject getImage() const;

        vk::Format getFormat() const;
        vk::ImageViewType getImageViewType() const;
        vk::ImageSubresourceRange getSubresourceRange() const;
        vk::ImageSubresourceLayers getSubresourceLayers() const;

        std::type_index getType() const override;

    private:
        vk::ImageView handle{};
        SharedDevice device{};
        ImageViewOptions options{};
        vk::ImageSubresourceRange subresourceRange{};
        EventToken rebindToken{};
        EventToken recreateToken{};

        void recreate(Object &image);
    };

    using UniqueImageView = std::unique_ptr<ImageView>;
    using SharedImageView = std::shared_ptr<ImageView>;
    using WeakImageView = std::weak_ptr<ImageView>;
}


#endif //LAVAVK_IMAGEVIEW_H
