//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_PIPELINE_H
#define LAVAVK_PIPELINE_H

#include <vulkan/vulkan.hpp>
#include "lava/framework/object.h"
#include "lava/framework/dependency-injection/dicontainer.h"
#include "lava/framework/dependency-injection/injectioncontext.h"
#include "lava/framework/core/device.h"
#include "lava/framework/rendering/pipelinestate.h"

namespace LavaVk::Core
{
    class Pipeline : public virtual Object
    {
    public:
        explicit Pipeline(InjectionContext &context);
        Pipeline(const Pipeline &) = delete;
        Pipeline(Pipeline &&) = delete;
        ~Pipeline() override;

        Pipeline &operator=(const Pipeline &) = delete;
        Pipeline &operator=(Pipeline &&) = delete;

        vk::Pipeline getHandle() const;
        const PipelineState &getState() const;

    protected:
        SharedDevice device;
        vk::Pipeline handle;
        PipelineState state;
    };

    using UniquePipeline = std::unique_ptr<Pipeline>;
    using SharedPipeline = std::shared_ptr<Pipeline>;
    using WeakPipeline = std::weak_ptr<Pipeline>;
}


#endif //LAVAVK_PIPELINE_H
