//
// Created by dorian on 10. 12. 2019..
//

#ifndef LAVAVK_BUFFERVIEWOPTIONS_H
#define LAVAVK_BUFFERVIEWOPTIONS_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/core/buffer.h"

namespace LavaVk::Core
{
    class BufferViewOptions : public virtual Object
    {
    public:
        BufferViewOptions() = default;
        BufferViewOptions(SharedBuffer buffer, vk::DeviceSize offset, vk::DeviceSize size, vk::DeviceSize n, vk::Format format);

        SharedBuffer buffer;
        /// Offset in bytes
        vk::DeviceSize offset;
        /// Size of single element in bytes.
        vk::DeviceSize size;
        /// Number of elements.
        vk::DeviceSize n;
        vk::Format format;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_BUFFERVIEWOPTIONS_H
