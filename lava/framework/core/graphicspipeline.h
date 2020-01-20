//
// Created by dorian on 13. 12. 2019..
//

#ifndef LAVAVK_GRAPHICSPIPELINE_H
#define LAVAVK_GRAPHICSPIPELINE_H

#include "pipeline.h"

namespace LavaVk::Core
{
    class GraphicsPipeline : public Pipeline
    {
    public:
        explicit GraphicsPipeline(InjectionContext& context);

        std::type_index getType() const override;
    };

    using UniqueGraphicsPipeline = std::unique_ptr<GraphicsPipeline>;
    using SharedGraphicsPipeline = std::shared_ptr<GraphicsPipeline>;
    using WeakGraphicsPipeline = std::weak_ptr<GraphicsPipeline>;
}


#endif //LAVAVK_GRAPHICSPIPELINE_H
