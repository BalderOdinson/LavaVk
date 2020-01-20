//
// Created by dorian on 12. 12. 2019..
//

#ifndef LAVAVK_FRAMEBUFFER_H
#define LAVAVK_FRAMEBUFFER_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "device.h"

namespace LavaVk::Core
{
    class Framebuffer : public virtual Object
    {
    public:
        explicit Framebuffer(InjectionContext &context);
        Framebuffer(const Framebuffer &) = delete;
        Framebuffer(Framebuffer &&) = delete;
        ~Framebuffer() override;

        Framebuffer &operator=(const Framebuffer &) = delete;
        Framebuffer &operator=(Framebuffer &&) = delete;

        vk::Framebuffer getHandle() const;

        std::type_index getType() const override;

    private:
        SharedDevice device;
        vk::Framebuffer handle;
    };

    using UniqueFramebuffer = std::unique_ptr<Framebuffer>;
    using SharedFramebuffer = std::shared_ptr<Framebuffer>;
    using WeakFramebuffer = std::weak_ptr<Framebuffer>;
}


#endif //LAVAVK_FRAMEBUFFER_H
