//
// Created by dorian on 12. 12. 2019..
//

#include "rendertargetoptions.h"
#include "lava/framework/platform/application.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/image.h"
#include "lava/framework/core/options/imageoptions.h"
#include "lava/framework/multisamplingoptions.h"

#include <utility>

LavaVk::RenderTargetOptions::RenderTargetOptions(std::vector<Core::SharedImageObject> images) : images(
        std::move(images))
{
}

std::type_index LavaVk::RenderTargetOptions::getType() const
{
    return typeid(RenderTargetOptions);
}

void LavaVk::RenderTargetOptions::create(const LavaVk::Core::SharedImageObject &swapchainImage)
{
    auto samples = Application::instance->container.option<MultisamplingOptions>()->samples;
    if (samples != vk::SampleCountFlagBits::e1)
    {
        auto colorImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
                vma::MemoryUsage::eGpuOnly, false,
                swapchainImage->getExtent(),
                swapchainImage->getFormat(),
                vk::ImageUsageFlagBits::eColorAttachment |
                vk::ImageUsageFlagBits::eTransferSrc,
                false, vk::MemoryPropertyFlagBits{},
                vk::MemoryPropertyFlagBits{},
                samples);
        outputAttachments.push_back(0);
        images.push_back(colorImage);
        create(swapchainImage->getExtent(), samples);
        resolveAttachments.push_back(images.size());
        images.push_back(swapchainImage);
        auto inputIt = inputAttachments.begin();
        for (size_t i = 1; i < outputAttachments.size(); ++i)
        {
            if (images[outputAttachments[i]]->getUsage() & vk::ImageUsageFlagBits::eColorAttachment)
            {
                auto inIt = std::find(inputIt, inputAttachments.end(), outputAttachments[i]);
                auto imageUsage =
                        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment;
                if (inIt != inputAttachments.end())
                {
                    imageUsage |= vk::ImageUsageFlagBits::eInputAttachment;
                    *inIt = images.size();
                    inputIt = inIt;
                }
                auto resolveImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
                        vma::MemoryUsage::eGpuOnly, false,
                        images[outputAttachments[i]]->getExtent(),
                        images[outputAttachments[i]]->getFormat(), imageUsage, false);
                resolveAttachments.push_back(images.size());
                images.push_back(resolveImage);
            }
        }
    }
    else
    {
        images.push_back(swapchainImage);
        outputAttachments.push_back(0);
        create(swapchainImage->getExtent(), samples);
    }
}

void LavaVk::RenderTargetOptions::recreate(const LavaVk::Core::SharedImageObject &swapchainImage)
{
    images.clear();
    inputAttachments.clear();
    outputAttachments.clear();
    resolveAttachments.clear();

    auto samples = Application::instance->container.option<MultisamplingOptions>()->samples;
    if (samples != vk::SampleCountFlagBits::e1)
    {
        auto colorImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
                vma::MemoryUsage::eGpuOnly, false,
                swapchainImage->getExtent(),
                swapchainImage->getFormat(),
                vk::ImageUsageFlagBits::eColorAttachment |
                vk::ImageUsageFlagBits::eTransientAttachment,
                false, vk::MemoryPropertyFlagBits{},
                vk::MemoryPropertyFlagBits{},
                samples);
        outputAttachments.push_back(0);
        images.push_back(colorImage);
        create(swapchainImage->getExtent(), samples);
        resolveAttachments.push_back(images.size());
        images.push_back(swapchainImage);
        auto inputIt = inputAttachments.begin();
        for (size_t i = 1; i < outputAttachments.size(); ++i)
        {
            if (images[outputAttachments[i]]->getUsage() & vk::ImageUsageFlagBits::eColorAttachment)
            {
                auto inIt = std::find(inputIt, inputAttachments.end(), outputAttachments[i]);
                auto imageUsage =
                        vk::ImageUsageFlagBits::eColorAttachment | vk::ImageUsageFlagBits::eTransientAttachment;
                if (inIt != inputAttachments.end())
                {
                    imageUsage |= vk::ImageUsageFlagBits::eInputAttachment;
                    *inIt = images.size();
                    inputIt = inIt;
                }
                auto resolveImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
                        vma::MemoryUsage::eGpuOnly, false,
                        images[outputAttachments[i]]->getExtent(),
                        images[outputAttachments[i]]->getFormat(), imageUsage, false);
                resolveAttachments.push_back(images.size());
                images.push_back(resolveImage);
            }
        }
    }
    else
    {
        images.push_back(swapchainImage);
        outputAttachments.push_back(0);
        create(swapchainImage->getExtent(), samples);
    }
}

std::type_index LavaVk::DefaultRenderTargetOptions::getType() const
{
    return typeid(DefaultRenderTargetOptions);
}

void LavaVk::DefaultRenderTargetOptions::create(vk::Extent3D swapchainExtent, vk::SampleCountFlagBits samples)
{
    auto depthImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eD32Sfloat,
            vk::ImageUsageFlagBits::eDepthStencilAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits{},
            samples);

    images.push_back(depthImage);
}

std::type_index LavaVk::DeferredRenderTargetOptions::getType() const
{
    return typeid(DeferredRenderTargetOptions);
}

void LavaVk::DeferredRenderTargetOptions::create(vk::Extent3D swapchainExtent, vk::SampleCountFlagBits samples)
{
    auto depthImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eD32Sfloat,
            vk::ImageUsageFlagBits::eDepthStencilAttachment |
            vk::ImageUsageFlagBits::eInputAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits::eLazilyAllocated,
            samples);

    auto albedoImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eR8G8B8A8Unorm,
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits::eLazilyAllocated,
            samples);

    auto normalImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eA2R10G10B10UnormPack32,
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits::eLazilyAllocated,
            samples);

    images.push_back(depthImage);
    images.push_back(albedoImage);
    images.push_back(normalImage);
    outputAttachments.emplace_back(1);
    outputAttachments.emplace_back(2);
    outputAttachments.emplace_back(3);
    inputAttachments.emplace_back(1);
    inputAttachments.emplace_back(2);
    inputAttachments.emplace_back(3);
}

std::type_index LavaVk::DeferredSpecularRenderTargetOptions::getType() const
{
    return typeid(DeferredSpecularRenderTargetOptions());
}

void LavaVk::DeferredSpecularRenderTargetOptions::create(vk::Extent3D swapchainExtent, vk::SampleCountFlagBits samples)
{
    auto depthImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eD32Sfloat,
            vk::ImageUsageFlagBits::eDepthStencilAttachment |
            vk::ImageUsageFlagBits::eInputAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits::eLazilyAllocated,
            samples);

    auto albedoImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eR8G8B8A8Unorm,
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits::eLazilyAllocated,
            samples);

    auto specularImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eR8G8B8A8Unorm,
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits::eLazilyAllocated,
            samples);

    auto normalImage = Application::instance->container.resolve<Core::Image, Core::ImageOptions>(
            vma::MemoryUsage::eGpuOnly, false,
            swapchainExtent,
            vk::Format::eA2R10G10B10UnormPack32,
            vk::ImageUsageFlagBits::eColorAttachment |
            vk::ImageUsageFlagBits::eInputAttachment |
            vk::ImageUsageFlagBits::eTransientAttachment,
            false, vk::MemoryPropertyFlagBits{},
            vk::MemoryPropertyFlagBits::eLazilyAllocated,
            samples);

    images.push_back(depthImage);
    images.push_back(albedoImage);
    images.push_back(specularImage);
    images.push_back(normalImage);

    outputAttachments.emplace_back(1);
    outputAttachments.emplace_back(2);
    outputAttachments.emplace_back(3);
    outputAttachments.emplace_back(4);
    inputAttachments.emplace_back(1);
    inputAttachments.emplace_back(2);
    inputAttachments.emplace_back(3);
    inputAttachments.emplace_back(4);
}
