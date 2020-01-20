//
// Created by dorian on 16. 12. 2019..
//

#ifndef LAVAVK_RENDERPIPELINEOPTIONS_H
#define LAVAVK_RENDERPIPELINEOPTIONS_H

#include <vector>
#include "lava/framework/object.h"
#include "lava/framework/rendering/subpass.h"

namespace LavaVk
{
    class RenderPipelineOptions : public virtual Object
    {
    public:
        RenderPipelineOptions() = default;
        explicit RenderPipelineOptions(std::vector<SharedSubpass> subpasses);

        std::vector<SharedSubpass> subpasses;
        void addSubpass(SharedSubpass subpass);

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_RENDERPIPELINEOPTIONS_H
