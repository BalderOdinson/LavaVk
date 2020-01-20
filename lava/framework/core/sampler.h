//
// Created by dorian on 11. 12. 2019..
//

#ifndef LAVAVK_SAMPLER_H
#define LAVAVK_SAMPLER_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "device.h"
#include "options/sampleroptions.h"

namespace LavaVk::Core
{
    class Sampler : public virtual Object
    {
    public:
        explicit Sampler(InjectionContext &context);
        explicit Sampler(const SamplerOptions &options);
        Sampler(const Sampler &) = delete;
        Sampler(Sampler &&) = delete;
        ~Sampler() override;

        Sampler &operator=(const Sampler &) = delete;
        Sampler &operator=(Sampler &&) = delete;

        vk::Sampler getHandle() const;

        std::type_index getType() const override;

    private:
        SharedDevice device;
        vk::Sampler handle;
    };

    using UniqueSampler = std::unique_ptr<Sampler>;
    using SharedSampler = std::shared_ptr<Sampler>;
    using WeakSampler = std::weak_ptr<Sampler>;
}


#endif //LAVAVK_SAMPLER_H
