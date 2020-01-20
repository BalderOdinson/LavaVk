//
// Created by dorian on 10. 12. 2019..
//

#include "bufferviewoptions.h"

#include <utility>

LavaVk::Core::BufferViewOptions::BufferViewOptions(SharedBuffer buffer, vk::DeviceSize offset, vk::DeviceSize size,
                                                   vk::DeviceSize n, vk::Format format) : buffer(std::move(buffer)),
                                                                                          offset(offset), size(size),
                                                                                          n(n), format(format)
{
}

std::type_index LavaVk::Core::BufferViewOptions::getType() const
{
    return typeid(BufferViewOptions);
}

