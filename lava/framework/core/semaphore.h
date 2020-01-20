//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_SEMAPHORE_H
#define LAVAVK_SEMAPHORE_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "device.h"

namespace LavaVk::Core
{
    class Semaphore : public virtual Object
    {
    public:
        explicit Semaphore(InjectionContext &context);
        Semaphore(const Semaphore &) = delete;
        Semaphore(Semaphore &&) = delete;
        ~Semaphore() override;

        Semaphore &operator=(const Semaphore &) = delete;
        Semaphore &operator=(Semaphore &&) = delete;

        vk::Semaphore getHandle() const;

        std::type_index getType() const override;

    private:
        vk::Semaphore handle;
        SharedDevice device;
    };

    using UniqueSemaphore = std::unique_ptr<Semaphore>;
    using SharedSemaphore = std::shared_ptr<Semaphore>;
    using WeakSemaphore = std::weak_ptr<Semaphore>;
}


#endif //LAVAVK_SEMAPHORE_H
