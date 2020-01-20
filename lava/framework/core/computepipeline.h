//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_COMPUTEPIPELINE_H
#define LAVAVK_COMPUTEPIPELINE_H

#include "pipeline.h"

namespace LavaVk::Core
{
    class ComputePipeline : public Pipeline
    {
    public:
        explicit ComputePipeline(InjectionContext &context);

        std::type_index getType() const override;
    };

    using UniqueComputePipeline = std::unique_ptr<ComputePipeline>;
    using SharedComputePipeline = std::shared_ptr<ComputePipeline>;
    using WeakComputePipeline = std::weak_ptr<ComputePipeline>;
}


#endif //LAVAVK_COMPUTEPIPELINE_H
