//
// Created by dorian on 17. 12. 2019..
//

#ifndef LAVAVK_RESOURCEBINDINGSTATE_H
#define LAVAVK_RESOURCEBINDINGSTATE_H

#include <vulkan/vulkan.hpp>
#include "utils.h"
#include "vkcommon.h"
#include "core/buffer.h"
#include "core/imageview.h"
#include "core/sampler.h"
#include "core/bufferview.h"

namespace LavaVk
{
    struct ResourceInfo
    {
        bool dirty{false};
        Core::SharedBuffer buffer{nullptr};
        vk::DeviceSize offset{0};
        vk::DeviceSize range{0};
        Core::SharedImageView imageView{nullptr};
        Core::SharedSampler sampler{nullptr};
        Core::SharedBufferView bufferView{nullptr};
    };

    class SetBindings
    {
    public:
        void reset();
        [[nodiscard]] bool isDirty() const;
        void clearDirty();
        void clearDirty(uint32_t binding, uint32_t arrayElement);
        void bindBuffer(const Core::SharedBuffer &buffer, vk::DeviceSize offset, vk::DeviceSize range, uint32_t binding, uint32_t arrayElement);
        void bindBufferView(const Core::SharedBufferView &bufferView, uint32_t binding, uint32_t arrayElement);
        void bindImage(const Core::SharedImageView &imageView, const Core::SharedSampler &sampler, uint32_t binding, uint32_t arrayElement);
        void bindInput(const Core::SharedImageView &imageView, uint32_t binding, uint32_t arrayElement);
        [[nodiscard]] const BindingMap<ResourceInfo> &getResourceBindings() const;

    private:
        bool dirty{false};
        BindingMap<ResourceInfo> resourceBindings;
    };

    class ResourceBindingState
    {
    public:
        void reset();
        bool isDirty();
        void clearDirty();
        void clearDirty(uint32_t set);
        void bindBuffer(const Core::SharedBuffer &buffer, vk::DeviceSize offset, vk::DeviceSize range, uint32_t set, uint32_t binding, uint32_t arrayElement);
        void bindBufferView(const Core::SharedBufferView &bufferView, uint32_t set, uint32_t binding, uint32_t arrayElement);
        void bindImage(const Core::SharedImageView &imageView, const Core::SharedSampler &sampler, uint32_t set, uint32_t binding, uint32_t arrayElement);
        void bindInput(const Core::SharedImageView &imageView, uint32_t set, uint32_t binding, uint32_t arrayElement);
        const std::unordered_map<uint32_t, SetBindings> &getSetBindings();

    private:
        bool dirty{false};
        std::unordered_map<uint32_t, SetBindings> setBindings;
    };
}


#endif //LAVAVK_RESOURCEBINDINGSTATE_H
