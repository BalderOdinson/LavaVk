//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_FENCEPOOL_H
#define LAVAVK_FENCEPOOL_H

#include <limits>
#include <numeric>
#include "lava/framework/object.h"
#include "core/device.h"
#include "core/fence.h"
#include "core/options/fenceoptions.h"

namespace LavaVk
{
    class FencePool : public virtual Object
    {
    public:
        explicit FencePool(InjectionContext &context);
        FencePool(const FencePool &) = delete;
        FencePool(FencePool &&) = delete;
        ~FencePool() override;

        FencePool &operator=(const FencePool &) = delete;
        FencePool &operator=(FencePool &&) = delete;

        Core::SharedFence requestFence(Core::FenceState desiredState = Core::FenceState::Unsignaled);
        bool wait(uint64_t timeout = (std::numeric_limits<uint64_t>::max)()) const;
        void reset();

        std::type_index getType() const override;

    private:
        Core::SharedDevice device;
        std::vector<Core::SharedFence> fences;
        uint32_t activeFenceCount{0};
    };

    using UniqueFencePool = std::unique_ptr<FencePool>;
    using SharedFencePool = std::shared_ptr<FencePool>;
    using WeakFencePool = std::weak_ptr<FencePool>;
}


#endif //LAVAVK_FENCEPOOL_H
