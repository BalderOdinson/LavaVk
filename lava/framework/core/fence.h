//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_FENCE_H
#define LAVAVK_FENCE_H

#include <vulkan/vulkan.hpp>
#include <numeric>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "device.h"

namespace LavaVk::Core
{
    class Fence : public virtual Object
    {
    public:
        explicit Fence(InjectionContext &context);
        Fence(const Fence &) = delete;
        Fence(Fence &&) = delete;
        ~Fence() override;

        Fence &operator=(const Fence &) = delete;
        Fence &operator=(Fence &&) = delete;

        bool wait(uint32_t timeout = std::numeric_limits<uint32_t>::max()) const;
        void reset() const;

        vk::Fence getHandle() const;

        template<typename BeginFences, typename EndFences>
        static bool
        waitForFences(BeginFences begin, EndFences end, uint64_t timeout = std::numeric_limits<uint64_t>::max());

        template<typename BeginFences, typename EndFences>
        static void resetFences(BeginFences begin, EndFences end);

        std::type_index getType() const override;

    private:
        SharedDevice device;
        vk::Fence handle;
    };

    using UniqueFence = std::unique_ptr<Fence>;
    using SharedFence = std::shared_ptr<Fence>;
    using WeakFence = std::weak_ptr<Fence>;

    template<typename BeginFences, typename EndFences>
    bool Fence::waitForFences(BeginFences begin, EndFences end, uint64_t timeout)
    {
        auto size = std::distance(begin, end);
        if (size == 0)
            return true;
        SharedDevice device = (*begin)->device;
        std::vector<vk::Fence> fences(size);
        std::transform(begin, end, fences.begin(), [](const SharedFence &fence)
        { return fence->handle; });
        return device->getHandle().waitForFences(fences, true, timeout) == vk::Result::eSuccess;;
    }

    template<typename BeginFences, typename EndFences>
    void Fence::resetFences(BeginFences begin, EndFences end)
    {
        auto size = std::distance(begin, end);
        if (size == 0)
            return;
        SharedDevice device = (*begin)->device;
        std::vector<vk::Fence> fences(size);
        std::transform(begin, end, fences.begin(), [](const SharedFence &fence)
        { return fence->handle; });
        device->getHandle().resetFences(fences);
    }

}


#endif //LAVAVK_FENCE_H
