//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_SCENEIMAGE_H
#define LAVAVK_SCENEIMAGE_H

#include <vulkan/vulkan.hpp>
#include <gli/texture.hpp>
#include "lava/framework/images/mipmapsoptions.h"
#include "lava/framework/scene-graph/component.h"
#include "lava/framework/core/image.h"
#include "lava/framework/core/imageview.h"
#include "lava/framework/core/buffer.h"
#include "lava/framework/core/commandbuffer.h"
#include "lava/framework/images/imagesource.h"

namespace LavaVk
{
    /// \brief Mipmap information
    struct Mipmap
    {
        /// Mipmap level
        uint32_t level = 0;
        /// Byte offset used for uploading
        uint32_t offset = 0;
        /// Width depth and height of the mipmap
        vk::Extent3D extent = {0, 0, 0};
    };

    class Image;

    using UniqueImage = std::unique_ptr<Image>;
    using SharedImage = std::shared_ptr<Image>;
    using WeakImage = std::weak_ptr<Image>;

    class Image : public Component
    {
    public:

        Image(SharedImageSource imageSource, std::string_view name = "");
        Image(SharedImageSource imageSource, SharedMipmapsOptions mipmapsOptions, std::string_view name = "");
        Image(const Image &) = delete;
        Image(Image &&) = delete;

        Image &operator=(const Image &) = delete;
        Image &operator=(Image &&) = delete;

        std::type_index getType() const override;
        vk::Format getFormat() const;
        const vk::Extent3D &getExtent() const;
        uint32_t getLayers() const;
        const std::vector<Mipmap> &getMipmaps() const;
        const std::vector<std::vector<vk::DeviceSize>> &getOffsets() const;
        Core::SharedImage getVkImage() const;
        Core::SharedImageView getVkImageView() const;
        SharedMipmapsOptions getMipmapsOptions() const;

        bool isValid();
        void load(const Core::SharedCommandBuffer &commandBuffer);

    protected:
        void generateMipmaps(gli::texture &tex);
        void extractData(gli::texture &tex);
        void createResources(gli::texture &tex);

    private:
        Core::SharedDevice device;
        SharedLogger logger;
        bool canGetLost{false};
        vk::Format format{};
        uint32_t layers{1};
        std::vector<Mipmap> mipmaps{{}};
        // Offsets stored like offsets[array_layer][mipmap_layer]
        std::vector<std::vector<vk::DeviceSize>> offsets;
        Core::SharedImage vkImage;
        Core::SharedImageView vkImageView;
        SharedImageSource imageSource;
        SharedMipmapsOptions mipmapsOptions;
    };
}


#endif //LAVAVK_SCENEIMAGE_H
