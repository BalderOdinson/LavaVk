//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_DESCRIPTORSETLAYOUT_H
#define LAVAVK_DESCRIPTORSETLAYOUT_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include <unordered_map>
#include <string_view>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "options/shaderresource.h"
#include "options/descriptorsetlayoutoptions.h"
#include "device.h"

namespace LavaVk::Core
{
    class DescriptorSetLayout : public virtual Object
    {
    public:
        explicit DescriptorSetLayout(InjectionContext &context);
        explicit DescriptorSetLayout(const DescriptorSetLayoutOptions &options);
        DescriptorSetLayout(const DescriptorSetLayout &) = delete;
        DescriptorSetLayout(DescriptorSetLayout &&) = delete;
        ~DescriptorSetLayout() override;

        DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;
        DescriptorSetLayout &operator=(DescriptorSetLayout &&) = delete;

        vk::DescriptorSetLayout getHandle() const;
        const std::vector<vk::DescriptorSetLayoutBinding> &getBindings() const;

        bool tryGetLayoutBinding(uint32_t bindingIndex, vk::DescriptorSetLayoutBinding &binding) const;
        bool tryGetLayoutBinding(const std::string &name, vk::DescriptorSetLayoutBinding &binding) const;

        std::type_index getType() const override;

    private:
        SharedDevice device;
        vk::DescriptorSetLayout handle;
        std::vector<vk::DescriptorSetLayoutBinding> bindings;
        std::unordered_map<uint32_t, vk::DescriptorSetLayoutBinding> bindingsLookup;
        std::unordered_map<std::string, uint32_t> resourcesLookup;
    };

    using UniqueDescriptorSetLayout = std::unique_ptr<DescriptorSetLayout>;
    using SharedDescriptorSetLayout = std::shared_ptr<DescriptorSetLayout>;
    using WeakDescriptorSetLayout = std::weak_ptr<DescriptorSetLayout>;
}


#endif //LAVAVK_DESCRIPTORSETLAYOUT_H
