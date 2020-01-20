//
// Created by dorian on 05. 01. 2020..
//

#ifndef LAVAVK_GEOMETRYRENDEROPTIONS_H
#define LAVAVK_GEOMETRYRENDEROPTIONS_H

#include "lava/framework/scene-graph/components/material.h"
#include "lava/framework/object.h"

namespace LavaVk
{
    class GeometryRenderOptions : public virtual Object
    {
    public:
        GeometryRenderOptions() = default;
        explicit GeometryRenderOptions(AlphaMode nodeMode);

        AlphaMode nodeMode{AlphaMode::Opaque};

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_GEOMETRYRENDEROPTIONS_H
