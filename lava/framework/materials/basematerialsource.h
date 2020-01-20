//
// Created by dorian on 04. 01. 2020..
//

#ifndef LAVAVK_BASEMATERIALSOURCE_H
#define LAVAVK_BASEMATERIALSOURCE_H

#include "materialsource.h"

namespace LavaVk
{
    class BaseMaterialSource : public MaterialSource
    {
    public:
        explicit BaseMaterialSource(SharedMaterial material);

        SharedMaterial load(const Core::SharedCommandBuffer &cmd) override;
        std::type_index getType() const override;

    private:
        SharedMaterial material;
    };
}


#endif //LAVAVK_BASEMATERIALSOURCE_H
