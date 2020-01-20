//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_DESCRIPTORPOOL_H
#define LAVAVK_DESCRIPTORPOOL_H

#include <vulkan/vulkan.hpp>
#include <vector>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "descriptorset.h"
#include "device.h"

namespace LavaVk::Core
{
    class DescriptorPool : public virtual Object
    {
    public:
        friend class DescriptorSet;

        explicit DescriptorPool(InjectionContext& context);
        DescriptorPool(const DescriptorPool &) = delete;
        DescriptorPool(DescriptorPool &&) = delete;
        ~DescriptorPool() override;

        DescriptorPool &operator=(const DescriptorPool &) = delete;
        DescriptorPool &operator=(DescriptorPool &&) = delete;

        void reset();

        const SharedDescriptorSetLayout &getDescriptorSetLayout() const;
        void setDescriptorSetLayout(const SharedDescriptorSetLayout &setLayout);


        std::type_index getType() const override;

    protected:
        vk::DescriptorSet allocate();
        void free(const vk::DescriptorSet& set);


    private:
        SharedDevice device;
        SharedDescriptorSetLayout descriptorSetLayout;
        // Descriptor pool size
        std::vector<vk::DescriptorPoolSize> poolSizes;
        // Number of sets to allocate for each pool
        uint32_t poolMaxSets{0};
        // Total descriptor pools created
        std::vector<vk::DescriptorPool> pools;
        // Count sets for each pool
        std::vector<uint32_t> poolSetsCount;
        // Current pool index to allocate descriptor set
        uint32_t poolIndex{0};
        // Map between descriptor set and pool index
        std::unordered_map<VkDescriptorSet, uint32_t> setPoolMapping;
        // Find next pool index or create new pool
        uint32_t findAvailablePool(uint32_t poolIndex);

    };
}



#endif //LAVAVK_DESCRIPTORPOOL_H
