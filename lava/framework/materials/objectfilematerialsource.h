//
// Created by dorian on 03. 01. 2020..
//

#ifndef LAVAVK_OBJECTFILEMATERIALSOURCE_H
#define LAVAVK_OBJECTFILEMATERIALSOURCE_H

#include "materialsource.h"
#include "lava/third-party/tiny.h"

namespace LavaVk
{
    class ObjectFileMaterialSource : public MaterialSource
    {
    public:
        explicit ObjectFileMaterialSource(tinyobj::material_t material, std::string_view baseDir = "");

        SharedMaterial load(const Core::SharedCommandBuffer &cmd) override;
        std::type_index getType() const override;

    private:
        tinyobj::material_t materialSource;
        std::string baseDir;
    };
}


#endif //LAVAVK_OBJECTFILEMATERIALSOURCE_H
