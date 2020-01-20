//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_SEMAPHOREPOOL_H
#define LAVAVK_SEMAPHOREPOOL_H

#include <lava/framework/core/semaphore.h>
#include "object.h"
#include "dependency-injection/dicontainer.h"
#include "dependency-injection/injectioncontext.h"
#include "core/device.h"
#include "semaphore.h"

namespace LavaVk
{
    class SemaphorePool : public virtual Object
    {
    public:
        explicit SemaphorePool(InjectionContext &context);
        SemaphorePool(const SemaphorePool &) = delete;
        SemaphorePool(SemaphorePool &&) = delete;

        SemaphorePool &operator=(const SemaphorePool &) = delete;
        SemaphorePool &operator=(SemaphorePool &&) = delete;

        Core::SharedSemaphore requestSemaphore();
        void reset();
        uint32_t getActiveSemaphoreCount() const;

        std::type_index getType() const override;

    private:
        Core::SharedDevice device;
        std::vector<Core::SharedSemaphore> semaphores;
        uint32_t activeSemaphoreCount{0};
    };

    using UniqueSemaphorePool = std::unique_ptr<SemaphorePool>;
    using SharedSemaphorePool = std::shared_ptr<SemaphorePool>;
    using WeakSemaphorePool = std::weak_ptr<SemaphorePool>;
}

#endif //LAVAVK_SEMAPHOREPOOL_H
