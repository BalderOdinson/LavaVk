//
// Created by dorian on 17. 12. 2019..
//

#ifndef LAVAVK_GEOMETRYSUBPASSOPTIONS_H
#define LAVAVK_GEOMETRYSUBPASSOPTIONS_H

#include "lava/framework/rendering/options/subpassoptions.h"
#include "lava/framework/scene-graph/scene.h"
#include "lava/framework/scene-graph/components/camera.h"
#include "lava/framework/rendering/subpasses/componentrenderer.h"

namespace LavaVk
{
    class GeometrySubpassOptions : public SubpassOptions
    {
    public:
        GeometrySubpassOptions() = default;
        explicit GeometrySubpassOptions(std::vector<SharedComponentRenderer> renderers);
        std::vector<SharedComponentRenderer> renderers;

        std::type_index getType() const override;
    };
}


#endif //LAVAVK_GEOMETRYSUBPASSOPTIONS_H
