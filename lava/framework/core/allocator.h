//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_ALLOCATOR_H
#define LAVAVK_ALLOCATOR_H

#include <vulkan/vulkan.hpp>
#include <utility>
#include <vk_mem_alloc.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "device.h"

namespace LavaVk::Core
{
    class Allocator : public virtual Object
    {
    public:
        explicit Allocator(InjectionContext &context);
        Allocator(const Allocator &) = delete;
        Allocator(Allocator &&) = delete;
        ~Allocator() override;

        Allocator &operator=(const Allocator &) = delete;
        Allocator &operator=(Allocator &&) = delete;

        vma::Allocator getHandle() const;
        SharedDevice getDevice() const;

        std::type_index getType() const override;

    private:
        SharedDevice device{nullptr};
        vma::Allocator handle;

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC
        static vma::VulkanFunctions bindVulkanFunctions(const vk::DispatchLoaderDynamic &dl);
#endif
    };

    using UniqueAllocator = std::unique_ptr<Allocator>;
    using SharedAllocator = std::shared_ptr<Allocator>;
    using WeakAllocator = std::weak_ptr<Allocator>;
}


#endif //LAVAVK_ALLOCATOR_H
