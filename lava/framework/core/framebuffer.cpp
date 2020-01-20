//
// Created by dorian on 12. 12. 2019..
//

#include "framebuffer.h"
#include "options/framebufferoptions.h"
#include "lava/framework/platform/application.h"

LavaVk::Core::Framebuffer::Framebuffer(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    auto options = context.container.option<FramebufferOptions>(context);

    auto &extent = options->renderTarget->getExtent();

    std::vector<vk::ImageView> attachments;

    for (auto &view : options->renderTarget->getViews())
        attachments.emplace_back(view->getHandle());


    vk::FramebufferCreateInfo createInfo{};

    createInfo.renderPass = options->renderPass->getHandle();
    createInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    createInfo.pAttachments = attachments.data();
    createInfo.width = extent.width;
    createInfo.height = extent.height;
    createInfo.layers = 1;

    handle = device->getHandle().createFramebuffer(createInfo);
}

LavaVk::Core::Framebuffer::~Framebuffer()
{
    if(handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}

vk::Framebuffer LavaVk::Core::Framebuffer::getHandle() const
{
    return handle;
}

std::type_index LavaVk::Core::Framebuffer::getType() const
{
    return typeid(Framebuffer);
}
