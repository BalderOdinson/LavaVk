//
// Created by dorian on 09. 12. 2019..
//

#ifndef LAVAVK_ALLOCATION_H
#define LAVAVK_ALLOCATION_H

#include "lava/framework/object.h"
#include "allocator.h"

namespace LavaVk::Core
{
    class Allocation : public virtual Object
    {
    public:
        Allocation() = default;
        Allocation(const Allocation &) = delete;
        Allocation(Allocation &&) = delete;

        Allocation &operator=(const Allocation &) = delete;
        Allocation &operator=(Allocation &&) = delete;

        [[nodiscard]] virtual vma::Allocation getAllocation() const = 0;
        [[nodiscard]] virtual SharedAllocator getAllocator() const = 0;
        [[nodiscard]] virtual vma::AllocationInfo getAllocationInfo() const;
        [[nodiscard]] bool touch() const;

        virtual void recreate() = 0;
        virtual void rebind() = 0;

        void invalidate(vk::DeviceSize offset, vk::DeviceSize size) const;
        void flush(vk::DeviceSize offset, vk::DeviceSize size) const;

        [[nodiscard]] void *map() const;
        void unmap() const;

        template<typename T>
        std::vector<T> data();

    protected:
        virtual void getData(void *data, size_t *size) const = 0;
    };

    template<typename T>
    std::vector<T> LavaVk::Core::Allocation::data()
    {
        size_t size;
        getData(nullptr, &size);
        auto n = size / sizeof(T);
        std::vector<T> result(n);
        getData(result.data(), &size);
        return result;
    }

    using UniqueAllocation = std::unique_ptr<Allocation>;
    using SharedAllocation = std::shared_ptr<Allocation>;
    using WeakAllocation = std::weak_ptr<Allocation>;
}


#endif //LAVAVK_ALLOCATION_H
