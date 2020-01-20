//
// Created by dorian on 08. 12. 2019..
//

#ifndef LAVAVK_SURFACE_H
#define LAVAVK_SURFACE_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "instance.h"

namespace LavaVk::Core
{
    class Surface : public virtual Object
    {
    public:
        explicit Surface(const vk::SurfaceKHR &surfaceKhr);
        explicit Surface(InjectionContext &context);
        Surface(const Surface &) = delete;
        Surface(Surface &&) = delete;
        ~Surface() override;

        Surface &operator=(const Surface &) = delete;
        Surface &operator=(Surface &&) = delete;

        vk::SurfaceKHR getHandle() const;
        SharedInstance getInstance() const;

        std::type_index getType() const override;

    private:
        vk::SurfaceKHR handle{nullptr};
        SharedInstance instance;
    };

    using UniqueSurface = std::unique_ptr<Surface>;
    using SharedSurface = std::shared_ptr<Surface>;
    using WeakSurface = std::weak_ptr<Surface>;
}


#endif //LAVAVK_SURFACE_H
