//
// Created by dorian on 13. 12. 2019..
//

#include "image.h"
#include "lava/framework/platform/application.h"
#include <gli/gli.hpp>
#include <gli/generate_mipmaps.hpp>
#include <gli/convert.hpp>
#include <utility>
#include "lava/framework/core/options/stagingbufferoptions.h"
#include "lava/framework/core/options/imageoptions.h"
#include "lava/framework/core/options/imageviewoptions.h"
#include "lava/framework/core/pipelinebarrier.h"


vk::ImageViewType getImageViewType(gli::target t)
{
    switch (t)
    {
        case gli::TARGET_1D:
            return vk::ImageViewType::e1D;
        case gli::TARGET_1D_ARRAY:
            return vk::ImageViewType::e1DArray;
        case gli::TARGET_2D:
            return vk::ImageViewType::e2D;
        case gli::TARGET_RECT:
        case gli::TARGET_RECT_ARRAY:
        case gli::TARGET_2D_ARRAY:
            return vk::ImageViewType::e2DArray;
        case gli::TARGET_3D:
            return vk::ImageViewType::e3D;
        case gli::TARGET_CUBE:
            return vk::ImageViewType::eCube;
        case gli::TARGET_CUBE_ARRAY:
            return vk::ImageViewType::eCubeArray;
        default:
            throw std::runtime_error("Unsupported image type!");
    }
}

gli::texture genMipmaps(gli::texture &tex)
{
    switch (tex.target())
    {
        case gli::TARGET_1D:
        {
            gli::texture1d tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_1D_ARRAY:
        {
            gli::texture1d_array tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_2D:
        {
            gli::texture2d tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_2D_ARRAY:
        {
            gli::texture2d_array tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_3D:
        {
            gli::texture3d tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_RECT:
        {
            gli::texture2d_array tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_RECT_ARRAY:
        {
            gli::texture2d_array tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_CUBE:
        {
            gli::texture_cube tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        case gli::TARGET_CUBE_ARRAY:
        {
            gli::texture_cube_array tt(tex);
            return gli::generate_mipmaps(tt, gli::filter::FILTER_LINEAR);
        }
        default:
            throw std::runtime_error("Unsupported image type!");
    }
}

gli::texture convertToFormat(gli::texture &tex, vk::Format format)
{
    switch (tex.target())
    {
        case gli::TARGET_1D:
        {
            gli::texture1d tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_1D_ARRAY:
        {
            gli::texture1d_array tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_2D:
        {
            gli::texture2d tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_2D_ARRAY:
        {
            gli::texture2d_array tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_3D:
        {
            gli::texture3d tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_RECT:
        {
            gli::texture2d_array tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_RECT_ARRAY:
        {
            gli::texture2d_array tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_CUBE:
        {
            gli::texture_cube tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        case gli::TARGET_CUBE_ARRAY:
        {
            gli::texture_cube_array tt(tex);
            return gli::convert(tt, static_cast<gli::format>(format));
        }
        default:
            throw std::runtime_error("Unsupported image type!");
    }
}

LavaVk::Image::Image(LavaVk::SharedImageSource imageSource, std::string_view name) : Component(name),
                                                                                     imageSource(std::move(imageSource))
{
    device = Application::instance->container.resolve<Core::Device>();
    logger = Application::instance->container.resolve<Logger>();
    mipmapsOptions = Application::instance->container.option<MipmapsOptions>();
}

LavaVk::Image::Image(LavaVk::SharedImageSource imageSource, LavaVk::SharedMipmapsOptions mipmapsOptions,
                     std::string_view name) : Component(name), imageSource(std::move(imageSource)),
                                              mipmapsOptions(std::move(mipmapsOptions))
{
    device = Application::instance->container.resolve<Core::Device>();
    logger = Application::instance->container.resolve<Logger>();
}

std::type_index LavaVk::Image::getType() const
{
    return typeid(Image);
}

vk::Format LavaVk::Image::getFormat() const
{
    return format;
}

const vk::Extent3D &LavaVk::Image::getExtent() const
{
    return mipmaps.front().extent;
}

uint32_t LavaVk::Image::getLayers() const
{
    return layers;
}

const std::vector<LavaVk::Mipmap> &LavaVk::Image::getMipmaps() const
{
    return mipmaps;
}

const std::vector<std::vector<vk::DeviceSize>> &LavaVk::Image::getOffsets() const
{
    return offsets;
}

LavaVk::Core::SharedImage LavaVk::Image::getVkImage() const
{
    return vkImage;
}

LavaVk::Core::SharedImageView LavaVk::Image::getVkImageView() const
{
    return vkImageView;
}

bool LavaVk::Image::isValid()
{
    return vkImage && vkImage->touch();
}

void LavaVk::Image::generateMipmaps(gli::texture &tex)
{
    if (tex.levels() > 1 || mipmapsOptions->maxLod == 0)
        return;

    auto mipTex = gli::texture(tex.target(), tex.format(), tex.extent(), tex.layers(), tex.faces(),
                               std::min<size_t>(gli::levels(tex.extent()), mipmapsOptions->maxLod));

    for (size_t layer = 0; layer < tex.layers(); ++layer)
        for (size_t face = 0; face < tex.faces(); ++face)
            memcpy(mipTex.data(layer, face, 0), tex.data(layer, face, 0), tex.size(0));

    tex = mipTex;

    tex = genMipmaps(tex);
    mipmaps.resize(tex.levels());

    size_t offset = 0;
    for (size_t level = 0; level < tex.levels(); ++level)
    {
        auto extent = tex.extent(level);
        mipmaps[level].extent = vk::Extent3D(extent.x, extent.y, extent.z);
        mipmaps[level].level = level;
        mipmaps[level].offset = offset;
        offset += tex.size(level);
    }

    offset = 0;

    if (layers > 1)
    {
        for (size_t layer = 0; layer < tex.layers(); ++layer)
        {
            for (size_t face = 0; face < tex.faces(); ++face)
            {
                std::vector<vk::DeviceSize> mipmapOffsets(tex.levels());
                for (size_t level = 0; level < tex.levels(); ++level)
                {
                    mipmapOffsets[level] = offset;
                    offset += tex.size(level);
                }
                offsets[layer * tex.faces() + face] = std::move(mipmapOffsets);
            }
        }
    }
}

void LavaVk::Image::load(const LavaVk::Core::SharedCommandBuffer &commandBuffer)
{
    if (!commandBuffer->isRecording())
        throw std::runtime_error("Command buffer must be in recording state!");

    auto tex = imageSource->load();
    extractData(tex);
    generateMipmaps(tex);
    createResources(tex);

    auto stagingBuffer = Application::instance->container.resolve<LavaVk::Core::Buffer, LavaVk::Core::StagingBufferOptions>(
            1,
            tex.size());

    stagingBuffer->update(tex.data(), tex.size(), 0, {}, {}, {}, {});
    commandBuffer->saveExecutionResource(stagingBuffer);

    if (layers == 1)
    {
        commandBuffer->pipelineBarrier().addImageMemoryBarrier(vkImage->getHandle(),
                                                               {},
                                                               vk::AccessFlagBits::eTransferWrite,
                                                               vk::PipelineStageFlagBits::eTopOfPipe,
                                                               vk::PipelineStageFlagBits::eTransfer,
                                                               vk::ImageLayout::eUndefined,
                                                               vk::ImageLayout::eTransferDstOptimal,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               vk::ImageAspectFlagBits::eColor).execute();
        Core::Buffer2ImageTransfer transfer = commandBuffer->copy(stagingBuffer, vkImage);
        for (auto &mipmap : mipmaps)
            transfer.addRegion(mipmap.offset, vk::ImageAspectFlagBits::eColor, mipmap.level, 0, 1, {},
                               mipmap.extent);
        transfer.setDestinationImageLayout(
                vk::ImageLayout::eTransferDstOptimal).execute();

        commandBuffer->pipelineBarrier().addImageMemoryBarrier(vkImage->getHandle(),
                                                               vk::AccessFlagBits::eTransferWrite,
                                                               vk::AccessFlagBits::eShaderRead,
                                                               vk::PipelineStageFlagBits::eTransfer,
                                                               vk::PipelineStageFlagBits::eFragmentShader,
                                                               vk::ImageLayout::eTransferDstOptimal,
                                                               vk::ImageLayout::eShaderReadOnlyOptimal,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               vk::ImageAspectFlagBits::eColor).execute();
    }
    else
    {
        commandBuffer->pipelineBarrier().addImageMemoryBarrier(vkImage->getHandle(),
                                                               {},
                                                               vk::AccessFlagBits::eTransferWrite,
                                                               vk::PipelineStageFlagBits::eTopOfPipe,
                                                               vk::PipelineStageFlagBits::eTransfer,
                                                               vk::ImageLayout::eUndefined,
                                                               vk::ImageLayout::eTransferDstOptimal,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               vk::ImageAspectFlagBits::eColor).execute();
        Core::Buffer2ImageTransfer transfer = commandBuffer->copy(stagingBuffer, vkImage);
        for (size_t layer = 0; layer < layers; ++layer)
        {
            for (size_t mipmap = 0; mipmap < mipmaps.size(); ++mipmap)
            {
                transfer.addRegion(offsets[layer][mipmap], vk::ImageAspectFlagBits::eColor, mipmaps[mipmap].level,
                                   layer, 1, {}, mipmaps[mipmap].extent);
            }
        }
        transfer.setDestinationImageLayout(
                vk::ImageLayout::eTransferDstOptimal).execute();

        commandBuffer->pipelineBarrier().addImageMemoryBarrier(vkImage->getHandle(),
                                                               vk::AccessFlagBits::eTransferWrite,
                                                               vk::AccessFlagBits::eShaderRead,
                                                               vk::PipelineStageFlagBits::eTransfer,
                                                               vk::PipelineStageFlagBits::eFragmentShader,
                                                               vk::ImageLayout::eTransferDstOptimal,
                                                               vk::ImageLayout::eShaderReadOnlyOptimal,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               VK_QUEUE_FAMILY_IGNORED,
                                                               vk::ImageAspectFlagBits::eColor).execute();
    }
}

void LavaVk::Image::extractData(gli::texture &tex)
{
    if (!device->isImageFormatSupported(static_cast<vk::Format >(tex.format())))
    {
        auto formatString = to_string(static_cast<vk::Format>(tex.format()));
        logger->warning(
                std::string_format("Format %s for image %s is not supported!", formatString.c_str(),
                                   getName().c_str()));
        tex = convertToFormat(tex, vk::Format::eR8G8B8A8Srgb);
    }

    format = static_cast<vk::Format>(tex.format());

    layers = tex.layers() * tex.faces();
    size_t offset = 0;

    offsets.resize(layers);
    mipmaps.resize(tex.levels());

    for (size_t level = 0; level < tex.levels(); ++level)
    {
        auto extent = tex.extent(level);
        mipmaps[level].extent = vk::Extent3D(extent.x, extent.y, extent.z);
        mipmaps[level].level = level;
        mipmaps[level].offset = offset;
        offset += tex.size(offset);
    }

    offset = 0;

    if (layers > 1)
    {
        for (size_t layer = 0; layer < tex.layers(); ++layer)
        {
            for (size_t face = 0; face < tex.faces(); ++face)
            {
                std::vector<vk::DeviceSize> mipmapOffsets(tex.levels());
                for (size_t level = 0; level < tex.levels(); ++level)
                {
                    mipmapOffsets[level] = offset;
                    offset += tex.size(level);
                }
                offsets[layer * tex.faces() + face] = std::move(mipmapOffsets);
            }
        }
    }
}

void LavaVk::Image::createResources(gli::texture &tex)
{
    auto options = new Core::ImageOptions();
    options->extent = getExtent();
    options->format = getFormat();
    options->memoryUsage = vma::MemoryUsage::eGpuOnly;
    options->canGetLost = canGetLost;
    options->mipLevels = mipmaps.size();
    options->arrayLayers = layers;
    options->imageUsage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
    if (gli::is_target_cube(tex.target()))
        options->flags |= vk::ImageCreateFlagBits::eCubeCompatible;
    else if (gli::is_target_array(tex.target()))
        options->flags |= vk::ImageCreateFlagBits::e2DArrayCompatible;
    vkImage = Application::instance->container.resolve<Core::Image>(options);

    auto viewType = getImageViewType(tex.target());
    vkImageView = Application::instance->container.resolve<Core::ImageView, Core::ImageViewOptions>(vkImage, viewType);
}

LavaVk::SharedMipmapsOptions LavaVk::Image::getMipmapsOptions() const
{
    return mipmapsOptions;
}
