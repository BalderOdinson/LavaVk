//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_QUEUE_H
#define LAVAVK_QUEUE_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/options/queueoptions.h"
#include "queuesubmitrequest.h"
#include "queuepresentrequest.h"

namespace LavaVk::Core
{
    class Queue : public virtual Object
    {
    public:
        explicit Queue(InjectionContext &context);

        vk::Queue getHandle() const;
        const QueueIndex &getIndex() const;
        vk::QueueFamilyProperties getProperties() const;
        bool supportPresent() const;
        QueueSubmitRequest createSubmitRequest();
        QueuePresentRequest createPresentRequest();
        void waitIdle() const;

        std::type_index getType() const override;

    private:
        vk::Queue handle{nullptr};
        QueueIndex index{0, 0};
        bool canPresent{false};
        vk::QueueFamilyProperties properties{};
    };

    using UniqueQueue = std::unique_ptr<Queue>;
    using SharedQueue = std::shared_ptr<Queue>;
    using WeakQueue = std::weak_ptr<Queue>;
}


#endif //LAVAVK_QUEUE_H
