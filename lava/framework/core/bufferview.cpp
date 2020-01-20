//
// Created by dorian on 10. 12. 2019..
//

#include "bufferview.h"

LavaVk::Core::BufferView::BufferView(LavaVk::InjectionContext &context)
{
    device = context.container.resolve<Device>();
    options = *context.container.option<BufferViewOptions>(context);

    vk::BufferViewCreateInfo info{};
    info.buffer = options.buffer->getHandle();
    info.format = options.format;
    info.offset = options.offset;
    info.range = options.size * options.n;

    handle = options.buffer->getAllocator()->getDevice()->getHandle().createBufferView(info);
    rebindToken = options.buffer->rebonded().subscribe(
            std::bind(&LavaVk::Core::BufferView::recreate, this, std::placeholders::_1));
    recreateToken = options.buffer->recreated().subscribe(
            std::bind(&LavaVk::Core::BufferView::recreate, this, std::placeholders::_1));
}

LavaVk::Core::BufferView::BufferView(LavaVk::Core::BufferViewOptions options)
        : device(options.buffer->getAllocator()->getDevice()), options(options)
{
    vk::BufferViewCreateInfo info{};
    info.buffer = options.buffer->getHandle();
    info.format = options.format;
    info.offset = options.offset;
    info.range = options.size * options.n;

    handle = device->getHandle().createBufferView(info);

    rebindToken = options.buffer->rebonded().subscribe(
            std::bind(&LavaVk::Core::BufferView::recreate, this, std::placeholders::_1));
    recreateToken = options.buffer->recreated().subscribe(
            std::bind(&LavaVk::Core::BufferView::recreate, this, std::placeholders::_1));
}

LavaVk::Core::BufferView::~BufferView()
{
    if(recreateToken)
        options.buffer->recreated().unsubscribe(recreateToken);
    if(rebindToken)
        options.buffer->rebonded().unsubscribe(rebindToken);

    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }
}


void LavaVk::Core::BufferView::recreate(LavaVk::Object &newBuffer)
{
    if (handle)
    {
        device->getHandle().destroy(handle);
        handle = nullptr;
    }

    vk::BufferViewCreateInfo info{};
    info.buffer = options.buffer->getHandle();
    info.format = options.format;
    info.offset = options.offset;
    info.range = options.size * options.n;

    handle = device->getHandle().createBufferView(info);
}

vk::BufferView LavaVk::Core::BufferView::getHandle() const
{
    return handle;
}

std::type_index LavaVk::Core::BufferView::getType() const
{
    return typeid(BufferView);
}

vk::Format LavaVk::Core::BufferView::getFormat() const
{
    return options.format;
}

vk::DeviceSize LavaVk::Core::BufferView::getSize() const
{
    return options.n;
}

vk::DeviceSize LavaVk::Core::BufferView::getSizeInBytes() const
{
    return options.size * options.n;
}

vk::DeviceSize LavaVk::Core::BufferView::getOffset() const
{
    return options.offset;
}

LavaVk::Core::SharedBuffer LavaVk::Core::BufferView::getBuffer() const
{
    return options.buffer;
}
