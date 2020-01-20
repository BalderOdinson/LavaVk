//
// Created by dorian on 02. 01. 2020..
//

#ifndef LAVAVK_MATERIALSOURCE_H
#define LAVAVK_MATERIALSOURCE_H

#include "lava/framework/object.h"
#include "lava/framework/scene-graph/components/material.h"
#include "lava/framework/core/commandbuffer.h"

namespace LavaVk
{
    class MaterialSource : public virtual Object
    {
    public:
        virtual SharedMaterial load(const Core::SharedCommandBuffer& cmd) = 0;
    };

    using UniqueMaterialSource = std::unique_ptr<MaterialSource>;
    using SharedMaterialSource = std::shared_ptr<MaterialSource>;
    using WeakMaterialSource = std::weak_ptr<MaterialSource>;
}


#endif //LAVAVK_MATERIALSOURCE_H
