//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_DESCRIPTORSET_H
#define LAVAVK_DESCRIPTORSET_H

#include "lava/framework/object.h"
#include "descriptorsetlayout.h"
#include "device.h"
#include "lava/framework/vkcommon.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/platform/logger.h"

namespace LavaVk::Core
{
    class DescriptorPool;

    using UniqueDescriptorPool = std::unique_ptr<DescriptorPool>;
    using SharedDescriptorPool = std::shared_ptr<DescriptorPool>;
    using WeakDescriptorPool = std::weak_ptr<DescriptorPool>;

    class DescriptorSet : public virtual Object
    {
    public:
        friend class DescriptorPool;

        explicit DescriptorSet(InjectionContext &context);
        DescriptorSet(const DescriptorSet &) = delete;
        DescriptorSet(DescriptorSet &&) = delete;
        ~DescriptorSet() override;

        DescriptorSet &operator=(const DescriptorSet &) = delete;
        DescriptorSet &operator=(DescriptorSet &&) = delete;

        void update(const BindingMap<vk::DescriptorBufferInfo> &bufferInfos,
                    const BindingMap<vk::BufferView> &bufferViewsInfos,
                    const BindingMap<vk::DescriptorImageInfo> &imageInfos);

        const SharedDescriptorSetLayout &getLayout() const;

        vk::DescriptorSet getHandle() const;

        BindingMap<vk::DescriptorBufferInfo> &getBufferInfos();
        BindingMap<vk::BufferView> &getBufferViewInfos();
        BindingMap<vk::DescriptorImageInfo> &getImageInfos();

        std::type_index getType() const override;

    private:
        SharedDevice device;
        SharedDescriptorSetLayout layout;
        SharedDescriptorPool pool;
        SharedLogger logger;
        vk::DescriptorSet handle;
        BindingMap<vk::DescriptorBufferInfo> bufferInfos;
        BindingMap<vk::BufferView> bufferViewsInfos;
        BindingMap<vk::DescriptorImageInfo> imageInfos;
    };

    using UniqueDescriptorSet = std::unique_ptr<DescriptorSet>;
    using SharedDescriptorSet = std::shared_ptr<DescriptorSet>;
    using WeakDescriptorSet = std::weak_ptr<DescriptorSet>;
}


#endif //LAVAVK_DESCRIPTORSET_H
