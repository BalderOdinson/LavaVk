//
// Created by dorian on 11. 12. 2019..
//

#include "rendertarget.h"
#include "options/rendertargetoptions.h"
#include "lava/framework/core/options/imageviewoptions.h"

LavaVk::Attachment::Attachment(vk::Format format, vk::SampleCountFlagBits samples,
                               const vk::ImageUsageFlags &usage)
        : format(format), samples(samples), usage(usage)
{
}

LavaVk::RenderTarget::RenderTarget(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Core::Device>();
    auto options = context.container.option<RenderTargetOptions>(context);
    images = options->images;
    inputAttachments = options->inputAttachments;
    outputAttachments = options->outputAttachments;
    resolveAttachments = options->resolveAttachments;
    if (images.empty())
        throw std::runtime_error("Should specify at least 1 image!");

    auto referent = vk::Extent2D(images.front()->getExtent().width, images.front()->getExtent().height);

    auto it = std::find_if_not(images.begin(), images.end(),
                               [&referent](Core::SharedImageObject &img)
                               {
                                   auto e = img->getExtent();
                                   return referent.width == e.width && referent.height == e.height;
                               });

    if (it != images.end())
        throw std::runtime_error("Extent size is not unique");

    extent = referent;

    for (auto &image : this->images)
    {
        if (image->getImageType() != vk::ImageType::e2D)
            throw std::runtime_error("Image type is not 2D");

        views.emplace_back(
                context.container.resolve<Core::ImageView, Core::ImageViewOptions>(image, vk::ImageViewType::e2D));
        attachments.emplace_back(Attachment{image->getFormat(), image->getSampleCount(), image->getUsage()});
    }
}

const vk::Extent2D &LavaVk::RenderTarget::getExtent() const
{
    return extent;
}

const std::vector<LavaVk::Core::SharedImageView> &LavaVk::RenderTarget::getViews() const
{
    return views;
}

const std::vector<LavaVk::Attachment> &LavaVk::RenderTarget::getAttachments() const
{
    return attachments;
}

void LavaVk::RenderTarget::setInputAttachments(std::vector<uint32_t> &input)
{
    inputAttachments = input;
}

const std::vector<uint32_t> &LavaVk::RenderTarget::getInputAttachments() const
{
    return inputAttachments;
}

void LavaVk::RenderTarget::setOutputAttachments(std::vector<uint32_t> &output)
{
    outputAttachments = output;
}

const std::vector<uint32_t> &LavaVk::RenderTarget::getOutputAttachments() const
{
    return outputAttachments;
}

void LavaVk::RenderTarget::setResolveAttachments(std::vector<uint32_t> &resolve)
{
    resolveAttachments = resolve;
}

const std::vector<uint32_t> &LavaVk::RenderTarget::getResolveAttachments() const
{
    return resolveAttachments;
}

std::type_index LavaVk::RenderTarget::getType() const
{
    return typeid(RenderTarget);
}

size_t LavaVk::RenderTarget::getHashCode() const
{
    size_t result = 0;
    for (auto &view : views)
        glm::detail::hash_combine(result, reinterpret_cast<size_t >(static_cast<VkImageView>(view->getHandle())));

    return result;
}
