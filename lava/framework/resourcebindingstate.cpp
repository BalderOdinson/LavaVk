//
// Created by dorian on 17. 12. 2019..
//

#include "resourcebindingstate.h"

void LavaVk::SetBindings::reset()
{
    clearDirty();
    resourceBindings.clear();
}

bool LavaVk::SetBindings::isDirty() const
{
    return dirty;
}

void LavaVk::SetBindings::clearDirty()
{
    dirty = false;
}

void LavaVk::SetBindings::clearDirty(uint32_t binding, uint32_t arrayElement)
{
    resourceBindings[binding][arrayElement].dirty = false;
}

void
LavaVk::SetBindings::bindBuffer(const LavaVk::Core::SharedBuffer &buffer, vk::DeviceSize offset, vk::DeviceSize range,
                                uint32_t binding, uint32_t arrayElement)
{
    resourceBindings[binding][arrayElement].dirty = true;
    resourceBindings[binding][arrayElement].buffer = buffer;
    resourceBindings[binding][arrayElement].offset = offset;
    resourceBindings[binding][arrayElement].range = range;

    dirty = true;
}

void LavaVk::SetBindings::bindImage(const LavaVk::Core::SharedImageView &imageView,
                                    const LavaVk::Core::SharedSampler &sampler, uint32_t binding, uint32_t arrayElement)
{
    resourceBindings[binding][arrayElement].dirty = true;
    resourceBindings[binding][arrayElement].imageView = imageView;
    resourceBindings[binding][arrayElement].sampler = sampler;

    dirty = true;
}

void
LavaVk::SetBindings::bindInput(const LavaVk::Core::SharedImageView &imageView, uint32_t binding, uint32_t arrayElement)
{
    resourceBindings[binding][arrayElement].dirty = true;
    resourceBindings[binding][arrayElement].imageView = imageView;

    dirty = true;
}

const LavaVk::BindingMap<LavaVk::ResourceInfo> &LavaVk::SetBindings::getResourceBindings() const
{
    return resourceBindings;
}

void LavaVk::SetBindings::bindBufferView(const LavaVk::Core::SharedBufferView &bufferView, uint32_t binding,
                                         uint32_t arrayElement)
{
    resourceBindings[binding][arrayElement].dirty = true;
    resourceBindings[binding][arrayElement].bufferView = bufferView;

    dirty = true;
}

void LavaVk::ResourceBindingState::reset()
{
    clearDirty();
    setBindings.clear();
}

bool LavaVk::ResourceBindingState::isDirty()
{
    return dirty;
}

void LavaVk::ResourceBindingState::clearDirty()
{
    dirty = false;
}

void LavaVk::ResourceBindingState::clearDirty(uint32_t set)
{
    setBindings[set].clearDirty();
}

void LavaVk::ResourceBindingState::bindBuffer(const LavaVk::Core::SharedBuffer &buffer, vk::DeviceSize offset,
                                              vk::DeviceSize range, uint32_t set, uint32_t binding,
                                              uint32_t arrayElement)
{
    setBindings[set].bindBuffer(buffer, offset, range, binding, arrayElement);
    dirty = true;
}

void LavaVk::ResourceBindingState::bindImage(const LavaVk::Core::SharedImageView &imageView,
                                             const LavaVk::Core::SharedSampler &sampler, uint32_t set, uint32_t binding,
                                             uint32_t arrayElement)
{
    setBindings[set].bindImage(imageView, sampler, binding, arrayElement);
    dirty = true;
}

void
LavaVk::ResourceBindingState::bindInput(const LavaVk::Core::SharedImageView &imageView, uint32_t set, uint32_t binding,
                                        uint32_t arrayElement)
{
    setBindings[set].bindInput(imageView, binding, arrayElement);
    dirty = true;
}

const std::unordered_map<uint32_t, LavaVk::SetBindings> &LavaVk::ResourceBindingState::getSetBindings()
{
    return setBindings;
}

void LavaVk::ResourceBindingState::bindBufferView(const LavaVk::Core::SharedBufferView &bufferView, uint32_t set,
                                                  uint32_t binding, uint32_t arrayElement)
{
    setBindings[set].bindBufferView(bufferView, binding, arrayElement);
    dirty = true;
}
