//
// Created by dorian on 03. 01. 2020..
//

#ifndef LAVAVK_PBRMATERIALSOURCE_H
#define LAVAVK_PBRMATERIALSOURCE_H

#include "materialsource.h"
#include "lava/framework/scene-graph/components/material.h"

namespace LavaVk
{
    class MemoryMaterialSource : public MaterialSource
    {
    public:
        explicit MemoryMaterialSource(SharedMaterial material);

        SharedMaterial load(const Core::SharedCommandBuffer &cmd) override;
        std::type_index getType() const override;

    private:
        SharedMaterial material;
    };
}



#endif //LAVAVK_PBRMATERIALSOURCE_H
